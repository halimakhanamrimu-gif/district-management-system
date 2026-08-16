#include <stdio.h>

void userPanel(void);
void restaurantOwner(void);
void mallOwner(void);
void clearInputBuffer(void);

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("============================================\n");
        printf("       DISTRICT MANAGEMENT SYSTEM\n");
        printf("============================================\n");

        printf("1. User Panel (Hospital)\n");
        printf("2. Restaurant Management\n");
        printf("3. Shopping Mall Management\n");
        printf("4. Exit\n");

        printf("--------------------------------------------\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf("\nInvalid input!\n");
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                userPanel();
                break;

            case 2:
                restaurantOwner();
                break;
            
            case 3:
                mallOwner();
                break;

            case 4:
                printf("\nThank you for using the system!\n");
                return 0;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}