#include <stdio.h>
#include <string.h>

#define MAX 100

struct Complaint
{
    int id;
    char name[50];
    char category[30];
    char description[200];
    int priority;
    char status[30];
};

struct Complaint complaints[MAX];
int count = 0;

/* Function to register a complaint */
void registerComplaint()
{
    if (count >= MAX)
    {
        printf("\nComplaint storage is full!\n");
        return;
    }

    complaints[count].id = count + 1;

    printf("\nEnter Student/Faculty Name: ");
    scanf(" %[^\n]", complaints[count].name);

    printf("Enter Category:\n");
    printf("1. Laboratory\n");
    printf("2. Classroom\n");
    printf("3. Hostel\n");
    printf("4. Transport\n");
    printf("5. Network\n");
    printf("6. Electricity\n");
    printf("7. Other\n");
    printf("Enter choice: ");

    int categoryChoice;
    scanf("%d", &categoryChoice);

    switch (categoryChoice)
    {
        case 1:
            strcpy(complaints[count].category, "Laboratory");
            break;
        case 2:
            strcpy(complaints[count].category, "Classroom");
            break;
        case 3:
            strcpy(complaints[count].category, "Hostel");
            break;
        case 4:
            strcpy(complaints[count].category, "Transport");
            break;
        case 5:
            strcpy(complaints[count].category, "Network");
            break;
        case 6:
            strcpy(complaints[count].category, "Electricity");
            break;
        default:
            strcpy(complaints[count].category, "Other");
    }

    printf("Enter Complaint Description: ");
    scanf(" %[^\n]", complaints[count].description);

    printf("\nSelect Priority:\n");
    printf("1. High\n");
    printf("2. Medium\n");
    printf("3. Low\n");
    printf("Enter priority: ");
    scanf("%d", &complaints[count].priority);

    strcpy(complaints[count].status, "Pending");

    printf("\nComplaint registered successfully!\n");
    printf("Complaint ID: %d\n", complaints[count].id);

    count++;
}

/* Function to display all complaints */
void displayComplaints()
{
    if (count == 0)
    {
        printf("\nNo complaints available.\n");
        return;
    }

    printf("\n========== ALL COMPLAINTS ==========\n");

    for (int i = 0; i < count; i++)
    {
        printf("\nComplaint ID : %d", complaints[i].id);
        printf("\nName         : %s", complaints[i].name);
        printf("\nCategory     : %s", complaints[i].category);
        printf("\nDescription  : %s", complaints[i].description);

        printf("\nPriority     : ");
        if (complaints[i].priority == 1)
            printf("High");
        else if (complaints[i].priority == 2)
            printf("Medium");
        else
            printf("Low");

        printf("\nStatus       : %s\n", complaints[i].status);
    }
}

/* Function to update complaint status */
void updateStatus()
{
    int id;
    printf("\nEnter Complaint ID: ");
    scanf("%d", &id);

    if (id < 1 || id > count)
    {
        printf("Invalid Complaint ID!\n");
        return;
    }

    printf("\nSelect New Status:\n");
    printf("1. Pending\n");
    printf("2. In Progress\n");
    printf("3. Resolved\n");
    printf("Enter choice: ");

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(complaints[id - 1].status, "Pending");
            break;

        case 2:
            strcpy(complaints[id - 1].status, "In Progress");
            break;

        case 3:
            strcpy(complaints[id - 1].status, "Resolved");
            break;

        default:
            printf("Invalid status choice!\n");
            return;
    }

    printf("Complaint status updated successfully!\n");
}

/* Function to search complaint */
void searchComplaint()
{
    int id;

    printf("\nEnter Complaint ID to search: ");
    scanf("%d", &id);

    if (id < 1 || id > count)
    {
        printf("Complaint not found!\n");
        return;
    }

    printf("\n========== COMPLAINT DETAILS ==========\n");
    printf("ID          : %d\n", complaints[id - 1].id);
    printf("Name        : %s\n", complaints[id - 1].name);
    printf("Category    : %s\n", complaints[id - 1].category);
    printf("Description : %s\n", complaints[id - 1].description);

    printf("Priority    : ");
    if (complaints[id - 1].priority == 1)
        printf("High\n");
    else if (complaints[id - 1].priority == 2)
        printf("Medium\n");
    else
        printf("Low\n");

    printf("Status      : %s\n", complaints[id - 1].status);
}

/* Function to generate summary */
void summary()
{
    int pending = 0;
    int progress = 0;
    int resolved = 0;

    int high = 0;
    int medium = 0;
    int low = 0;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(complaints[i].status, "Pending") == 0)
            pending++;
        else if (strcmp(complaints[i].status, "In Progress") == 0)
            progress++;
        else if (strcmp(complaints[i].status, "Resolved") == 0)
            resolved++;

        if (complaints[i].priority == 1)
            high++;
        else if (complaints[i].priority == 2)
            medium++;
        else
            low++;
    }

    printf("\n========== CAMPUS COMPLAINT SUMMARY ==========\n");

    printf("Total Complaints : %d\n", count);

    printf("\nStatus Summary:\n");
    printf("Pending          : %d\n", pending);
    printf("In Progress      : %d\n", progress);
    printf("Resolved         : %d\n", resolved);

    printf("\nPriority Summary:\n");
    printf("High Priority    : %d\n", high);
    printf("Medium Priority  : %d\n", medium);
    printf("Low Priority     : %d\n", low);
}

/* Main function */
int main()
{
    int choice;

    do
    {
        printf("\n\n====================================");
        printf("\n     SMART CAMPUS COMPLAINT SYSTEM");
        printf("\n====================================");

        printf("\n1. Register Complaint");
        printf("\n2. Display All Complaints");
        printf("\n3. Update Complaint Status");
        printf("\n4. Search Complaint");
        printf("\n5. Generate Summary");
        printf("\n6. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                registerComplaint();
                break;

            case 2:
                displayComplaints();
                break;

            case 3:
                updateStatus();
                break;

            case 4:
                searchComplaint();
                break;

            case 5:
                summary();
                break;

            case 6:
                printf("\nThank you for using Smart Campus Complaint System!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}
