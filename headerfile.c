#include <stdio.h>
#include <string.h>

#define MAX 100

struct Patient
{
    int id;
    char name[50];
    int age;
    int priority;
    char doctor[50];
    char bed[20];
    char status[30];
};

struct Patient patients[MAX];
int count = 0;

/* Register a new patient */
void registerPatient()
{
    if (count >= MAX)
    {
        printf("\nPatient storage is full!\n");
        return;
    }

    patients[count].id = count + 1;

    printf("\nEnter Patient Name: ");
    scanf(" %[^\n]", patients[count].name);

    printf("Enter Age: ");
    scanf("%d", &patients[count].age);

    printf("\nAssign Emergency Priority:\n");
    printf("1. Critical\n");
    printf("2. High\n");
    printf("3. Medium\n");
    printf("4. Low\n");
    printf("Enter priority: ");
    scanf("%d", &patients[count].priority);

    strcpy(patients[count].doctor, "Not Assigned");
    strcpy(patients[count].bed, "Not Assigned");
    strcpy(patients[count].status, "Waiting");

    printf("\nPatient registered successfully!");
    printf("\nPatient ID: %d\n", patients[count].id);

    count++;
}

/* Display patients according to priority */
void displayQueue()
{
    if (count == 0)
    {
        printf("\nNo patients in the emergency queue.\n");
        return;
    }

    struct Patient temp;

    /* Sort patients based on priority */
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (patients[i].priority > patients[j].priority)
            {
                temp = patients[i];
                patients[i] = patients[j];
                patients[j] = temp;
            }
        }
    }

    printf("\n========== EMERGENCY QUEUE ==========\n");

    for (int i = 0; i < count; i++)
    {
        printf("\nPatient ID : %d", patients[i].id);
        printf("\nName       : %s", patients[i].name);
        printf("\nAge        : %d", patients[i].age);

        printf("\nPriority   : ");

        if (patients[i].priority == 1)
            printf("Critical");
        else if (patients[i].priority == 2)
            printf("High");
        else if (patients[i].priority == 3)
            printf("Medium");
        else
            printf("Low");

        printf("\nDoctor     : %s", patients[i].doctor);
        printf("\nBed        : %s", patients[i].bed);
        printf("\nStatus     : %s\n", patients[i].status);
    }
}

/* Assign doctor and bed */
void assignDoctorBed()
{
    int id;

    printf("\nEnter Patient ID: ");
    scanf("%d", &id);

    if (id < 1 || id > count)
    {
        printf("Invalid Patient ID!\n");
        return;
    }

    printf("Enter Doctor Name: ");
    scanf(" %[^\n]", patients[id - 1].doctor);

    printf("Enter Bed Number: ");
    scanf(" %[^\n]", patients[id - 1].bed);

    strcpy(patients[id - 1].status, "Under Treatment");

    printf("\nDoctor and bed assigned successfully!\n");
}

/* Update patient status */
void updateStatus()
{
    int id;
    int choice;

    printf("\nEnter Patient ID: ");
    scanf("%d", &id);

    if (id < 1 || id > count)
    {
        printf("Invalid Patient ID!\n");
        return;
    }

    printf("\nSelect Patient Status:\n");
    printf("1. Waiting\n");
    printf("2. Under Treatment\n");
    printf("3. Observation\n");
    printf("4. Discharged\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(patients[id - 1].status, "Waiting");
            break;

        case 2:
            strcpy(patients[id - 1].status, "Under Treatment");
            break;

        case 3:
            strcpy(patients[id - 1].status, "Observation");
            break;

        case 4:
            strcpy(patients[id - 1].status, "Discharged");
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    printf("\nPatient status updated successfully!\n");
}

/* Find next patient */
void nextPatient()
{
    int selected = -1;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(patients[i].status, "Waiting") == 0)
        {
            if (selected == -1 ||
                patients[i].priority < patients[selected].priority)
            {
                selected = i;
            }
        }
    }

    if (selected == -1)
    {
        printf("\nNo waiting patients.\n");
        return;
    }

    printf("\n========== NEXT PATIENT ==========\n");

    printf("Patient ID : %d\n", patients[selected].id);
    printf("Name       : %s\n", patients[selected].name);

    printf("Priority   : ");

    if (patients[selected].priority == 1)
        printf("Critical\n");
    else if (patients[selected].priority == 2)
        printf("High\n");
    else if (patients[selected].priority == 3)
        printf("Medium\n");
    else
        printf("Low\n");

    printf("\nThis patient should be handled next.\n");
}

/* Generate emergency department summary */
void summary()
{
    int critical = 0;
    int high = 0;
    int medium = 0;
    int low = 0;

    int waiting = 0;
    int treatment = 0;
    int observation = 0;
    int discharged = 0;

    for (int i = 0; i < count; i++)
    {
        /* Priority count */
        if (patients[i].priority == 1)
            critical++;
        else if (patients[i].priority == 2)
            high++;
        else if (patients[i].priority == 3)
            medium++;
        else
            low++;

        /* Status count */
        if (strcmp(patients[i].status, "Waiting") == 0)
            waiting++;
        else if (strcmp(patients[i].status, "Under Treatment") == 0)
            treatment++;
        else if (strcmp(patients[i].status, "Observation") == 0)
            observation++;
        else if (strcmp(patients[i].status, "Discharged") == 0)
            discharged++;
    }

    printf("\n========== EMERGENCY SUMMARY ==========\n");

    printf("\nTotal Patients : %d\n", count);

    printf("\nPriority Summary:\n");
    printf("Critical       : %d\n", critical);
    printf("High           : %d\n", high);
    printf("Medium         : %d\n", medium);
    printf("Low            : %d\n", low);

    printf("\nPatient Status:\n");
    printf("Waiting        : %d\n", waiting);
    printf("Under Treatment: %d\n", treatment);
    printf("Observation    : %d\n", observation);
    printf("Discharged     : %d\n", discharged);
}

/* Main function */
int main()
{
    int choice;

    do
    {
        printf("\n\n========================================");
        printf("\n   SMART HOSPITAL EMERGENCY QUEUE");
        printf("\n========================================");

        printf("\n1. Register Patient");
        printf("\n2. Display Emergency Queue");
        printf("\n3. Find Next Patient");
        printf("\n4. Assign Doctor and Bed");
        printf("\n5. Update Patient Status");
        printf("\n6. Generate Emergency Summary");
        printf("\n7. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                registerPatient();
                break;

            case 2:
                displayQueue();
                break;

            case 3:
                nextPatient();
                break;

            case 4:
                assignDoctorBed();
                break;

            case 5:
                updateStatus();
                break;

            case 6:
                summary();
                break;

            case 7:
                printf("\nThank you for using the system!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}
