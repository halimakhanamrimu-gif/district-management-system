#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FILE_NAME "malls.txt"
#define TEMP_FILE "temp_mall.txt"

struct Mall
{
    char name[100];
    char location[100];
    char owner[100];
    char gmail[100];
    char password[100];
    float rating;
    int rating_count;
};

void addMall();
void viewMall();
void updateMall();
void deleteMall();
void mallOwner();
void buyerPanel();
void mallMenu();
static void clearBuffer();
static void inputString(char str[], int size, char message[]);
static int isValidEmail(const char* email);
static int isValidPassword(const char* password);

/* Clear input buffer */
static void clearBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/* Input string */
static void inputString(char str[], int size, char message[])
{
    printf("%s", message);

    fgets(str, size, stdin);

    str[strcspn(str, "\n")] = '\0';
}

/* Validation */
static int isValidEmail(const char* email)
{
    if (strchr(email, '@') != NULL && strstr(email, ".com") != NULL) {
        return 1;
    }
    return 0;
}

static int isValidPassword(const char* password)
{
    if (strlen(password) >= 6) {
        return 1;
    }
    return 0;
}

/* Add Mall */
void addMall()
{
    struct Mall mall;
    FILE *file;

    printf("\n+======================================================+\n");
    printf("|                  ADD SHOPPING MALL                   |\n");
    printf("+======================================================+\n");

    inputString(mall.name, 100,
                "Mall Name: ");

    inputString(mall.location, 100,
                "Location: ");

    inputString(mall.owner, 100,
                "Owner Name: ");

    while (1) {
        inputString(mall.gmail, 100,
                    "Owner Gmail (must contain @ and .com): ");
        if (isValidEmail(mall.gmail)) {
            break;
        }
        printf("Invalid email format! Please try again.\n");
    }

    while (1) {
        inputString(mall.password, 100,
                    "Owner Password (minimum 6 characters): ");
        if (isValidPassword(mall.password)) {
            break;
        }
        printf("Invalid password! Please try again.\n");
    }

    mall.rating = 0.0;
    mall.rating_count = 0;

    file = fopen(FILE_NAME, "a");

    if (file == NULL)
    {
        printf("Unable to open malls.txt\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%s|%.2f|%d\n",
            mall.name,
            mall.location,
            mall.owner,
            mall.gmail,
            mall.password,
            mall.rating,
            mall.rating_count);

    fclose(file);

    printf("\nShopping Mall added successfully!\n");
}

/* View Mall */
void viewMall()
{
    struct Mall mall;
    FILE *file;

    int count = 0;

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        printf("\nNo shopping mall information available.\n");
        return;
    }

    printf("\n+======================================================+\n");
    printf("|                  SHOPPING MALL LIST                  |\n");
    printf("+======================================================+\n");

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
        count++;

        printf("\nShopping Mall %d\n", count);
        printf("Name     : %s\n", mall.name);
        printf("Location : %s\n", mall.location);
        printf("Owner    : %s\n", mall.owner);
        printf("Gmail    : %s\n", mall.gmail);
        if (mall.rating_count > 0) {
            printf("Rating   : %.1f Stars (%d reviews)\n", mall.rating, mall.rating_count);
        } else {
            printf("Rating   : No ratings yet\n");
        }
    }

    if (count == 0)
    {
        printf("No shopping mall data found.\n");
    }

    fclose(file);
}


/* Update Mall */
void updateMall()
{
    char gmail[100];
    char password[100];

    struct Mall mall;

    FILE *file;
    FILE *temp;

    int found = 0;

    printf("\n+======================================================+\n");
    printf("|                 UPDATE SHOPPING MALL                 |\n");
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

            printf("\nLogin successful.\n");
            printf("Enter new information:\n\n");

            inputString(mall.name, 100,
                        "New Mall Name: ");

            inputString(mall.location, 100,
                        "New Location: ");

            inputString(mall.owner, 100,
                        "New Owner Name: ");

            while (1) {
                inputString(mall.password, 100,
                            "New Password (minimum 6 characters): ");
                if (isValidPassword(mall.password)) {
                    break;
                }
                printf("Invalid password! Please try again.\n");
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

        printf("\nShopping Mall updated successfully!\n");
    }
    else
    {
        remove(TEMP_FILE);

        printf("\nInvalid Gmail or Password!\n");
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
