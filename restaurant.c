#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "restaurants.txt"
#define TEMP_FILE "temp_restaurant.txt"

struct Restaurant
{
    char name[100];
    char location[100];
    char owner[100];
    char gmail[100];
    char password[100];
};

void addRestaurant();
void viewRestaurant();
void updateRestaurant();
void deleteRestaurant();
void restaurantOwner();
void clearBuffer();
void inputString(char str[], int size, char message[]);

void clearBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void inputString(char str[], int size, char message[])
{
    printf("%s", message);

    fgets(str, size, stdin);

    str[strcspn(str, "\n")] = '\0';
}

void addRestaurant()
{
    struct Restaurant restaurant;
    FILE *file;

    printf("\n====================================\n");
    printf("        ADD RESTAURANT\n");
    printf("====================================\n");

    inputString(restaurant.name, 100,
              "Restaurant Name: ");

    inputString(restaurant.location, 100,
                "Location: ");

    inputString(restaurant.owner, 100,
                "Owner Name: ");

    inputString(restaurant.gmail, 100,
                "Owner Gmail: ");

    inputString(restaurant.password, 100,
                "Owner Password: ");

    file = fopen(FILE_NAME, "a");

    if (file == NULL)
    {
        printf("Unable to open restaurants.txt\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%s\n",
            restaurant.name,
            restaurant.location,
            restaurant.owner,
            restaurant.gmail,
            restaurant.password);

    fclose(file);

    printf("\nRestaurant added successfully!\n");
}

void viewRestaurant()
{
    struct Restaurant restaurant;
    FILE *file;

    int count = 0;

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        printf("\nNo restaurant information available.\n");
        return;
    }

    printf("\n====================================\n");
    printf("        RESTAURANT LIST\n");
    printf("====================================\n");

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^\n]\n",
                  restaurant.name,
                  restaurant.location,
                  restaurant.owner,
                  restaurant.gmail,
                  restaurant.password) == 5)
    {
        count++;

        printf("\nRestaurant %d\n", count);
        printf("Name     : %s\n", restaurant.name);
        printf("Location : %s\n", restaurant.location);
        printf("Owner    : %s\n", restaurant.owner);
        printf("Gmail    : %s\n", restaurant.gmail);
    }

    if (count == 0)
    {
        printf("No restaurant data found.\n");
    }

    fclose(file);
}

void updateRestaurant()
{
    char gmail[100];
    char password[100];

    struct Restaurant restaurant;

    FILE *file;
    FILE *temp;

    int found = 0;

    printf("\n====================================\n");
    printf("       UPDATE RESTAURANT\n");
    printf("====================================\n");

    inputString(gmail, 100,
                "Enter Owner Gmail: ");

    inputString(password, 100,
                "Enter Owner Password: ");

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        printf("\nNo restaurant data found.\n");
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
                  restaurant.name,
                  restaurant.location,
                  restaurant.owner,
                  restaurant.gmail,
                  restaurant.password) == 5)
    {
        if (strcmp(gmail, restaurant.gmail) == 0 &&
            strcmp(password, restaurant.password) == 0)
        {
            found = 1;

            printf("\nLogin successful.\n");
            printf("Enter new information:\n\n");

            inputString(restaurant.name, 100,
                        "New Restaurant Name: ");

            inputString(restaurant.location, 100,
                        "New Location: ");

            inputString(restaurant.owner, 100,
                        "New Owner Name: ");

            inputString(restaurant.password, 100,
                        "New Password: ");
        }

        fprintf(temp, "%s|%s|%s|%s|%s\n",
                restaurant.name,
                restaurant.location,
                restaurant.owner,
                restaurant.gmail,
                restaurant.password);
    }

    fclose(file);
    fclose(temp);

    if (found)
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);

        printf("\nRestaurant updated successfully!\n");
    }
    else
    {
        remove(TEMP_FILE);

        printf("\nInvalid Gmail or Password!\n");
    }
}

void deleteRestaurant()
{
    char gmail[100];
    char password[100];
    char confirm;

    struct Restaurant restaurant;

    FILE *file;
    FILE *temp;

    int found = 0;

    printf("\n====================================\n");
    printf("       DELETE RESTAURANT\n");
    printf("====================================\n");

    inputString(gmail, 100,
                "Enter Owner Gmail: ");

    inputString(password, 100,
                "Enter Owner Password: ");

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        printf("\nNo restaurant data found.\n");
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
                  restaurant.name,
                  restaurant.location,
                  restaurant.owner,
                  restaurant.gmail,
                  restaurant.password) == 5)
    {
        if (strcmp(gmail, restaurant.gmail) == 0 &&
            strcmp(password, restaurant.password) == 0)
        {
            found = 1;

            continue;
        }

        fprintf(temp, "%s|%s|%s|%s|%s\n",
                restaurant.name,
                restaurant.location,
                restaurant.owner,
                restaurant.gmail,
                restaurant.password);
    }

    fclose(file);
    fclose(temp);

    if (!found)
    {
        remove(TEMP_FILE);

        printf("\nInvalid Gmail or Password!\n");

        return;
    }

    printf("\nAre you sure you want to delete this restaurant?\n");
    printf("Enter Y for Yes / N for No: ");

    scanf(" %c", &confirm);

    clearBuffer();

    if (confirm == 'Y' || confirm == 'y')
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);

        printf("\nRestaurant deleted successfully!\n");
    }
    else
    {
        remove(TEMP_FILE);

        printf("\nDelete operation cancelled.\n");
    }
}

void restaurantOwner()
{
    int choice;

    while (1)
    {
        printf("\n====================================\n");
        printf("       RESTAURANT OWNER PANEL\n");
        printf("====================================\n");

        printf("1. Add Restaurant\n");
        printf("2. View Restaurant\n");
        printf("3. Update Restaurant\n");
        printf("4. Delete Restaurant\n");
        printf("5. Back\n");

        printf("------------------------------------\n");
        printf("Enter Choice: ");

        scanf("%d", &choice);

        clearBuffer();

        switch (choice)
        {
            case 1:
                addRestaurant();
                break;

            case 2:
                viewRestaurant();
                break;

            case 3:
                updateRestaurant();
                break;

            case 4:
                deleteRestaurant();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid Choice!\n");
        }
    }
}