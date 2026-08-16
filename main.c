#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void userPanel(void);
void restaurantMenu(void);
void mallMenu(void);

static int mainReadInt(const char *prompt, int min, int max)
{
    int val;
    char line[32];

    while (1)
    {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL)
            continue;
        if (sscanf(line, "%d", &val) == 1 && val >= min && val <= max)
            return val;

        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
    }
}


/* Delete Mall */
void deleteMall()
{
    char gmail[100];
    char password[100];
    char confirm;

    struct Mall mall;

    FILE *file;
    FILE *temp;

    int found = 0;

    printf("\n+======================================================+\n");
    printf("|                 DELETE SHOPPING MALL                 |\n");
    printf("+======================================================+\n");

    inputString(gmail, 100,
                "Enter Owner Gmail: ");

    inputString(password, 100,
                "Enter Owner Password: ");

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        printf("\nNo shopping mall data found.\n");
        return;
    }

    temp = fopen(TEMP_FILE, "w");

    if (temp == NULL)
    {
        fclose(file);

        printf("Unable to create temporary file.\n");

        return;
    }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|%f|%d\n",
                  mall.name,
                  mall.location,
                  mall.owner,
                  mall.gmail,
                  mall.password,
                  &mall.rating,
                  &mall.rating_count) == 7)
    {
        if (strcmp(gmail, mall.gmail) == 0 &&
            strcmp(password, mall.password) == 0)
        {
            found = 1;

            continue;
        }

        fprintf(temp, "%s|%s|%s|%s|%s|%.2f|%d\n",
                mall.name,
                mall.location,
                mall.owner,
                mall.gmail,
                mall.password,
                mall.rating,
                mall.rating_count);
    }

    fclose(file);
    fclose(temp);

    if (!found)
    {
        remove(TEMP_FILE);

        printf("\nInvalid Gmail or Password!\n");

        return;
    }

    printf("\nAre you sure you want to delete this shopping mall?\n");
    printf("Enter Y for Yes / N for No: ");

    scanf(" %c", &confirm);

    clearBuffer();

    if (confirm == 'Y' || confirm == 'y')
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);

        printf("\nShopping Mall deleted successfully!\n");
    }
    else
    {
        remove(TEMP_FILE);

        printf("\nDelete operation cancelled.\n");
    }
}


/* Buyer Panel */
void buyerPanel()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("+======================================================+\n");
        printf("|                    BUYER PANEL                       |\n");
        printf("+======================================================+\n");
        printf("|  1. View All Shopping Malls                          |\n");
        printf("|  2. Rate a Shopping Mall                             |\n");
        printf("|  3. Back                                             |\n");
        printf("+------------------------------------------------------+\n");

        printf("Enter Choice: ");

        scanf("%d", &choice);

        clearBuffer();

        switch (choice)
        {
            case 1:
                viewMall();
                break;

            case 2:
            {
                char mallName[100];
                int ratingInput;
                struct Mall mall;
                FILE *file;
                FILE *temp;
                int found = 0;

                inputString(mallName, 100, "Enter Shopping Mall Name to rate: ");

                file = fopen(FILE_NAME, "r");
                if (file == NULL)
                {
                    printf("\nNo shopping mall data found.\n");
                    break;
                }

                temp = fopen(TEMP_FILE, "w");
                if (temp == NULL)
                {
                    fclose(file);
                    printf("Unable to create temporary file.\n");
                    break;
                }

                while (fscanf(file,
                              "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|%f|%d\n",
                              mall.name,
                              mall.location,
                              mall.owner,
                              mall.gmail,
                              mall.password,
                              &mall.rating,
                              &mall.rating_count) == 7)
                {
                    if (strcasecmp(mallName, mall.name) == 0 && !found)
                    {
                        found = 1;
                        printf("\nMall found! Enter rating (1 to 5 stars): ");
                        scanf("%d", &ratingInput);
                        clearBuffer();

                        if (ratingInput >= 1 && ratingInput <= 5) {
                            float totalRating = (mall.rating * mall.rating_count) + ratingInput;
                            mall.rating_count++;
                            mall.rating = totalRating / mall.rating_count;
                            printf("\nThank you! Rating updated successfully.\n");
                        } else {
                            printf("\nInvalid rating. Please enter a value between 1 and 5.\n");
                        }
                    }

                    fprintf(temp, "%s|%s|%s|%s|%s|%.2f|%d\n",
                            mall.name,
                            mall.location,
                            mall.owner,
                            mall.gmail,
                            mall.password,
                            mall.rating,
                            mall.rating_count);
                }

                fclose(file);
                fclose(temp);

                if (found)
                {
                    remove(FILE_NAME);
                    rename(TEMP_FILE, FILE_NAME);
                }
                else
                {
                    remove(TEMP_FILE);
                    printf("\nShopping Mall '%s' not found!\n", mallName);
                }
                break;
            }

            case 3:
                return;

            default:
                printf("\nInvalid Choice!\n");
        }
    }
}


/* Mall Owner Panel */
void mallOwner()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("+======================================================+\n");
        printf("|             SHOPPING MALL OWNER PANEL                |\n");
        printf("+======================================================+\n");
        printf("|  1. Add Shopping Mall                                |\n");
        printf("|  2. View Shopping Mall                               |\n");
        printf("|  3. Update Shopping Mall                             |\n");
        printf("|  4. Delete Shopping Mall                             |\n");
        printf("|  5. Back                                             |\n");
        printf("+------------------------------------------------------+\n");

        printf("Enter Choice: ");

        scanf("%d", &choice);

        clearBuffer();

        switch (choice)
        {
            case 1:
                addMall();
                break;

            case 2:
                viewMall();
                break;

            case 3:
                updateMall();
                break;

            case 4:
                deleteMall();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid Choice!\n");
        }
    }
}

/* Mall Menu */
void mallMenu()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("+======================================================+\n");
        printf("|                SHOPPING MALL MENU                    |\n");
        printf("+======================================================+\n");
        printf("|  1. Shopping Mall Owner Panel                        |\n");
        printf("|  2. Buyer's Panel                                    |\n");
        printf("|  3. Back to Main Menu                                |\n");
        printf("+------------------------------------------------------+\n");

        printf("Enter Choice: ");

        scanf("%d", &choice);

        clearBuffer();

        switch (choice)
        {
            case 1:
                mallOwner();
                break;

            case 2:
                buyerPanel();
                break;

            case 3:
                return;

            default:
                printf("\nInvalid Choice!\n");
        }
    }
}
