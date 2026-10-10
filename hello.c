#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REQUESTS 50
#define TEAM_COUNT 3

const char *teams[TEAM_COUNT] = {
    "Rescue-1", "Medical-1", "Fire-1"
};

typedef struct {
    int id;
    char location[50];
    int people;
    char level[16];
    char resources[100];
    char team[20];
    char status[20];
    int priority;
} Request;

Request requests[MAX_REQUESTS];
int requestCount = 0;

int getNumber(const char *prompt, int min, int max, int *value)
{
    char input[100];
    char *end;
    long number;

    for (;;) {
        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        errno = 0;
        number = strtol(input, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (end == input || *end != '\0' || errno == ERANGE ||
            number < min || number > max) {
            printf("Enter a number from %d to %d.\n", min, max);
            continue;
        }

        *value = (int)number;
        return 1;
    }
}

int getText(const char *prompt, char *text, size_t size)
{
    for (;;) {
        printf("%s", prompt);

        if (fgets(text, (int)size, stdin) == NULL) {
            return 0;
        }

        size_t length = strlen(text);

        if (length > 0 && text[length - 1] == '\n') {
            text[--length] = '\0';
        } else if (!feof(stdin)) {
            int character;
            while ((character = getchar()) != '\n' && character != EOF) {
                /* Discard extra input. */
            }
            printf("Input is too long. Try again.\n");
            continue;
        }

        while (length > 0 && isspace((unsigned char)text[length - 1])) {
            text[--length] = '\0';
        }

        if (length == 0) {
            printf("Please enter a value.\n");
            continue;
        }

        return 1;
    }
}

int findRequest(int id)
{
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].id == id) {
            return i;
        }
    }

    return -1;
}

void calculatePriority(Request *request)
{
    int levelPoints;
    int peoplePoints = request->people;
    int resourcePoints = 1;

    if (strcmp(request->level, "Critical") == 0) {
        levelPoints = 400;
    } else if (strcmp(request->level, "High") == 0) {
        levelPoints = 300;
    } else if (strcmp(request->level, "Medium") == 0) {
        levelPoints = 200;
    } else {
        levelPoints = 100;
    }

    for (int i = 0; request->resources[i] != '\0'; i++) {
        if (request->resources[i] == ',') {
            resourcePoints++;
        }
    }

    if (peoplePoints > 49) peoplePoints = 49;
    if (resourcePoints > 50) resourcePoints = 50;

    request->priority = levelPoints + peoplePoints + resourcePoints;
}

void registerRequest(void)
{
    Request *request;
    int id;
    int levelChoice;

    if (requestCount == MAX_REQUESTS) {
        printf("The request list is full.\n");
        return;
    }

    if (!getNumber("Request ID: ", 1, INT_MAX, &id)) return;

    if (findRequest(id) != -1) {
        printf("That ID is already being used.\n");
        return;
    }

    request = &requests[requestCount];
    request->id = id;

    if (!getText("Location: ", request->location,
                 sizeof(request->location))) return;

    if (!getNumber("Number of people affected: ", 1, 1000000,
                   &request->people)) return;

    printf("Emergency level: 1. Critical  2. High  3. Medium  4. Low\n");
    if (!getNumber("Choose level: ", 1, 4, &levelChoice)) return;

    if (levelChoice == 1) {
        strcpy(request->level, "Critical");
    } else if (levelChoice == 2) {
        strcpy(request->level, "High");
    } else if (levelChoice == 3) {
        strcpy(request->level, "Medium");
    } else {
        strcpy(request->level, "Low");
    }

    if (!getText("Required resources (comma-separated): ",
                 request->resources, sizeof(request->resources))) return;

    strcpy(request->team, "Unassigned");
    strcpy(request->status, "Pending");
    calculatePriority(request);

    requestCount++;
    printf("Request registered. Priority: %d\n", request->priority);
}

void printRequest(const Request *request)
{
    printf("\nID: %d | Location: %s | People: %d\n",
           request->id, request->location, request->people);
    printf("Level: %s | Resources: %s\n",
           request->level, request->resources);
    printf("Team: %s | Status: %s | Priority: %d\n",
           request->team, request->status, request->priority);
}

void displayByPriority(void)
{
    int order[MAX_REQUESTS];

    if (requestCount == 0) {
        printf("No requests registered.\n");
        return;
    }

    for (int i = 0; i < requestCount; i++) {
        order[i] = i;
    }

    for (int i = 0; i < requestCount - 1; i++) {
        for (int j = 0; j < requestCount - i - 1; j++) {
            if (requests[order[j]].priority <
                requests[order[j + 1]].priority) {
                int temporary = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temporary;
            }
        }
    }

    printf("\nRequests from highest priority to lowest:\n");
    for (int i = 0; i < requestCount; i++) {
        printRequest(&requests[order[i]]);
    }
}

int teamIsAvailable(int teamIndex, int requestIndex)
{
    for (int i = 0; i < requestCount; i++) {
        if (i == requestIndex) continue;

        if (strcmp(requests[i].team, teams[teamIndex]) == 0 &&
            strcmp(requests[i].status, "Completed") != 0 &&
            strcmp(requests[i].status, "Cancelled") != 0) {
            return 0;
        }
    }

    return 1;
}

void assignTeam(void)
{
    int id;
    int requestIndex;
    int available[TEAM_COUNT];
    int availableCount = 0;
    int choice;

    if (!getNumber("Request ID: ", 1, INT_MAX, &id)) return;

    requestIndex = findRequest(id);
    if (requestIndex == -1) {
        printf("Request not found.\n");
        return;
    }

    if (strcmp(requests[requestIndex].status, "Completed") == 0 ||
        strcmp(requests[requestIndex].status, "Cancelled") == 0) {
        printf("This request is already closed.\n");
        return;
    }

    for (int i = 0; i < TEAM_COUNT; i++) {
        if (teamIsAvailable(i, requestIndex)) {
            available[availableCount++] = i;
        }
    }

    if (availableCount == 0) {
        printf("No teams are available.\n");
        return;
    }

    printf("Available teams:\n");
    for (int i = 0; i < availableCount; i++) {
        printf("%d. %s\n", i + 1, teams[available[i]]);
    }

    if (!getNumber("Choose a team: ", 1, availableCount, &choice)) return;

    strcpy(requests[requestIndex].team, teams[available[choice - 1]]);
    printf("Team assigned.\n");
}

void updateStatus(void)
{
    int id;
    int requestIndex;
    int choice;

    if (!getNumber("Request ID: ", 1, INT_MAX, &id)) return;

    requestIndex = findRequest(id);
    if (requestIndex == -1) {
        printf("Request not found.\n");
        return;
    }

    printf("1. Pending  2. In Progress  3. Completed  4. Cancelled\n");
    if (!getNumber("Choose status: ", 1, 4, &choice)) return;

    if (choice == 1) {
        strcpy(requests[requestIndex].status, "Pending");
    } else if (choice == 2) {
        strcpy(requests[requestIndex].status, "In Progress");
    } else if (choice == 3) {
        strcpy(requests[requestIndex].status, "Completed");
    } else {
        strcpy(requests[requestIndex].status, "Cancelled");
    }

    printf("Status updated.\n");
}

void searchRequest(void)
{
    int id;
    int requestIndex;

    if (!getNumber("Request ID to search: ", 1, INT_MAX, &id)) return;

    requestIndex = findRequest(id);
    if (requestIndex == -1) {
        printf("Request not found.\n");
    } else {
        printRequest(&requests[requestIndex]);
    }
}

void showSummary(void)
{
    int critical = 0;
    int high = 0;
    int medium = 0;
    int low = 0;
    int active = 0;

    for (int i = 0; i < requestCount; i++) {
        if (strcmp(requests[i].level, "Critical") == 0) critical++;
        else if (strcmp(requests[i].level, "High") == 0) high++;
        else if (strcmp(requests[i].level, "Medium") == 0) medium++;
        else low++;

        if (strcmp(requests[i].status, "Completed") != 0 &&
            strcmp(requests[i].status, "Cancelled") != 0) {
            active++;
        }
    }

    printf("\nTotal requests: %d\n", requestCount);
    printf("Active requests: %d\n", active);
    printf("Critical: %d | High: %d | Medium: %d | Low: %d\n",
           critical, high, medium, low);
}

int main(void)
{
    int choice;

    for (;;) {
        printf("\n=== Emergency Rescue System ===\n");
        printf("1. Register request\n");
        printf("2. Display requests by priority\n");
        printf("3. Assign rescue team\n");
        printf("4. Update request status\n");
        printf("5. Search request\n");
        printf("6. Emergency summary\n");
        printf("0. Exit\n");

        if (!getNumber("Choose an option: ", 0, 6, &choice)) {
            break;
        }

        switch (choice) {
            case 1: registerRequest(); break;
            case 2: displayByPriority(); break;
            case 3: assignTeam(); break;
            case 4: updateStatus(); break;
            case 5: searchRequest(); break;
            case 6: showSummary(); break;
            case 0: return 0;
        }
    }

    return 0;
}
