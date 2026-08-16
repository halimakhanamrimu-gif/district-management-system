#include <stdio.h>
#include <stdlib.h>


void userPanel(void);
void restaurantMenu(void);
void mallMenu(void);

/* readInt is defined as static in each sub-module; we need our own copy here */
static int mainReadInt(const char *prompt, int min, int max)
{
    int  val;
    char line[32];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) continue;
        if (sscanf(line, "%d", &val) == 1 && val >= min && val <= max)
            return val;
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
    }
}

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("+======================================================================+\n");
        printf("|                                                                      |\n");
        printf("|             *** WELCOME TO DISTRICT MANAGEMENT SYSTEM ***            |\n");
        printf("|                                                                      |\n");
        printf("+======================================================================+\n");
        printf("         A smart way to manage the district's core facilities!          \n");
        printf("+----------------------------------------------------------------------+\n\n");

        printf("+======================================================+\n");
        printf("|                   MAIN DASHBOARD                     |\n");
        printf("+======================================================+\n");
        printf("|  1. User Panel (Hospital)                            |\n");
        printf("|  2. Restaurant                                       |\n");
        printf("|  3. Shopping Mall                                    |\n");
        printf("|  4. Exit                                             |\n");
        printf("+------------------------------------------------------+\n\n");

        choice = mainReadInt("=> Enter choice (1-4): ", 1, 4);

        switch (choice)
        {
            case 1:
                userPanel();
                break;

            case 2:
                restaurantMenu();
                break;

            case 3:
                mallMenu();
                break;

            case 4:
                printf("\nThank you for using the system!\n");
                return 0;
        }
    }

    return 0;
}
