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
};

void addMall();
void viewMall();
void updateMall();
void deleteMall();
void mallOwner();
void clearBuffer();
void inputString(char str[], int size, char message[]);


/* Clear input buffer */
void clearBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}


/* Input string */
void inputString(char str[], int size, char message[])
{
    printf("%s", message);

    fgets(str, size, stdin);

    str[strcspn(str, "\n")] = '\0';
}


/* Add Mall */
void addMall()
{
    struct Mall mall;
    FILE *file;

    printf("\n====================================\n");
    printf("        ADD SHOPPING MALL\n");
    printf("====================================\n");

    inputString(mall.name, 100,
                "Mall Name: ");

    inputString(mall.location, 100,
                "Location: ");

    inputString(mall.owner, 100,
                "Owner Name: ");

    inputString(mall.gmail, 100,
                "Owner Gmail: ");

    inputString(mall.password, 100,
                "Owner Password: ");

    file = fopen(FILE_NAME, "a");

    if (file == NULL)
    {
        printf("Unable to open malls.txt\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%s\n",
            mall.name,
            mall.location,
            mall.owner,
            mall.gmail,
            mall.password);

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

    printf("\n====================================\n");
    printf("        SHOPPING MALL LIST\n");
    printf("====================================\n");

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^\n]\n",
                  mall.name,
                  mall.location,
                  mall.owner,
                  mall.gmail,
                  mall.password) == 5)
    {
        count++;

        printf("\nShopping Mall %d\n", count);
        printf("Name     : %s\n", mall.name);
        printf("Location : %s\n", mall.location);
        printf("Owner    : %s\n", mall.owner);
        printf("Gmail    : %s\n", mall.gmail);
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

    printf("\n====================================\n");
    printf("       UPDATE SHOPPING MALL\n");
    printf("====================================\n");

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
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^\n]\n",
                  mall.name,
                  mall.location,
                  mall.owner,
                  mall.gmail,
                  mall.password) == 5)
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

            inputString(mall.password, 100,
                        "New Password: ");
        }

        fprintf(temp, "%s|%s|%s|%s|%s\n",
                mall.name,
                mall.location,
                mall.owner,
                mall.gmail,
                mall.password);
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

    printf("\n====================================\n");
    printf("       DELETE SHOPPING MALL\n");
    printf("====================================\n");

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
                  "%99[^|]|%99[^|]|%99[^|]|%99[^\n]\n",
                  mall.name,
                  mall.location,
                  mall.owner,
                  mall.gmail,
                  mall.password) == 5)
    {
        if (strcmp(gmail, mall.gmail) == 0 &&
            strcmp(password, mall.password) == 0)
        {
            found = 1;

            continue;
        }

        fprintf(temp, "%s|%s|%s|%s|%s\n",
                mall.name,
                mall.location,
                mall.owner,
                mall.gmail,
                mall.password);
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


/* Mall Owner Panel */
void mallOwner()
{
    int choice;

    while (1)
    {
        printf("\n====================================\n");
        printf("       SHOPPING MALL OWNER PANEL\n");
        printf("====================================\n");

        printf("1. Add Shopping Mall\n");
        printf("2. View Shopping Mall\n");
        printf("3. Update Shopping Mall\n");
        printf("4. Delete Shopping Mall\n");
        printf("5. Back\n");

        printf("------------------------------------\n");
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


/* Main */
int main()
{
    mallOwner();

    return 0;
}