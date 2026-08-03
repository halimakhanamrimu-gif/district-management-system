#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USER_FILE "users.txt"
#define TEMP_FILE "temp_users.txt"

/* ================================
   USER STRUCTURE
   ================================ */

struct User
{
    char name[100];
    char gmail[100];
    char password[100];
    char address[150];
    char phone[20];
    int age;
    char gender[20];
};


/* ================================
   FUNCTION PROTOTYPES
   ================================ */

void userPanel(void);
void registerUser(void);
void viewMyInformation(void);
void updateMyInformation(void);
void deleteMyInformation(void);

void removeNewline(char str[]);
void clearInputBuffer(void);

int gmailExists(const char gmail[]);

int findUser(const char gmail[],
            const char password[],
            struct User *foundUser);


/* ================================
   REMOVE NEWLINE
   ================================ */

void removeNewline(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}


/* ================================
   CLEAR INPUT BUFFER
   ================================ */

void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}


/* ================================
   CHECK GMAIL ALREADY EXISTS
   ================================ */

int gmailExists(const char gmail[])
{
    FILE *file;
    struct User user;

    file = fopen(USER_FILE, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name,
                  user.gmail,
                  user.password,
                  user.address,
                  user.phone,
                  &user.age,
                  user.gender) == 7)
    {
        if (strcmp(user.gmail, gmail) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;
}


/* ================================
   FIND USER BY GMAIL + PASSWORD
   ================================ */

int findUser(const char gmail[],
             const char password[],
             struct User *foundUser)
{
    FILE *file;
    struct User user;

    file = fopen(USER_FILE, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name,
                  user.gmail,
                  user.password,
                  user.address,
                  user.phone,
                  &user.age,
                  user.gender) == 7)
    {
        if (strcmp(gmail, user.gmail) == 0 &&
            strcmp(password, user.password) == 0)
        {
            *foundUser = user;

            fclose(file);

            return 1;
        }
    }

    fclose(file);

    return 0;
}


/* ================================
   REGISTER NEW USER
   ================================ */

void registerUser(void)
{
    struct User user;
    FILE *file;

    printf("\n");
    printf("============================================\n");
    printf("             USER REGISTRATION\n");
    printf("============================================\n");

    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    removeNewline(user.name);

    printf("Enter Gmail: ");
    fgets(user.gmail, sizeof(user.gmail), stdin);
    removeNewline(user.gmail);

    if (gmailExists(user.gmail))
    {
        printf("\nThis Gmail is already registered!\n");
        printf("Please use another Gmail.\n");

        return;
    }

    printf("Enter Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    removeNewline(user.password);

    printf("Enter Address: ");
    fgets(user.address, sizeof(user.address), stdin);
    removeNewline(user.address);

    printf("Enter Phone Number: ");
    fgets(user.phone, sizeof(user.phone), stdin);
    removeNewline(user.phone);

    printf("Enter Age: ");

    while (scanf("%d", &user.age) != 1 || user.age <= 0)
    {
        clearInputBuffer();
        printf("Invalid Age! Enter again: ");
    }

    clearInputBuffer();

    printf("Enter Gender: ");
    fgets(user.gender, sizeof(user.gender), stdin);
    removeNewline(user.gender);

    file = fopen(USER_FILE, "a");

    if (file == NULL)
    {
        printf("\nERROR: Unable to open users.txt!\n");
        return;
    }

    fprintf(file,
            "%s|%s|%s|%s|%s|%d|%s\n",
            user.name,
            user.gmail,
            user.password,
            user.address,
            user.phone,
            user.age,
            user.gender);

    fclose(file);

    printf("\n============================================\n");
    printf("       REGISTRATION SUCCESSFUL!\n");
    printf("============================================\n");
}


/* ================================
   VIEW MY INFORMATION
   ================================ */

void viewMyInformation(void)
{
    char gmail[100];
    char password[100];

    struct User user;

    printf("\n");
    printf("============================================\n");
    printf("          VERIFY YOUR IDENTITY\n");
    printf("============================================\n");

    printf("Enter your Gmail: ");
    fgets(gmail, sizeof(gmail), stdin);
    removeNewline(gmail);

    printf("Enter your Password: ");
    fgets(password, sizeof(password), stdin);
    removeNewline(password);

    if (findUser(gmail, password, &user))
    {
        printf("\n");
        printf("============================================\n");
        printf("             MY INFORMATION\n");
        printf("============================================\n");

        printf("Name    : %s\n", user.name);
        printf("Gmail   : %s\n", user.gmail);
        printf("Address : %s\n", user.address);
        printf("Phone   : %s\n", user.phone);
        printf("Age     : %d\n", user.age);
        printf("Gender  : %s\n", user.gender);

        printf("============================================\n");
    }
    else
    {
        printf("\n");
        printf("============================================\n");
        printf("     INVALID GMAIL OR PASSWORD!\n");
        printf("            ACCESS DENIED\n");
        printf("============================================\n");
    }
}


/* ================================
   UPDATE MY INFORMATION
   ================================ */

void updateMyInformation(void)
{
    char gmail[100];
    char password[100];

    struct User user;

    FILE *file;
    FILE *tempFile;

    int found = 0;

    printf("\n");
    printf("============================================\n");
    printf("           UPDATE MY INFORMATION\n");
    printf("============================================\n");

    printf("Enter your Gmail: ");
    fgets(gmail, sizeof(gmail), stdin);
    removeNewline(gmail);

    printf("Enter your Password: ");
    fgets(password, sizeof(password), stdin);
    removeNewline(password);

    file = fopen(USER_FILE, "r");

    if (file == NULL)
    {
        printf("\nNo user information found.\n");
        return;
    }

    tempFile = fopen(TEMP_FILE, "w");

    if (tempFile == NULL)
    {
        printf("\nUnable to create temporary file!\n");
        fclose(file);
        return;
    }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name,
                  user.gmail,
                  user.password,
                  user.address,
                  user.phone,
                  &user.age,
                  user.gender) == 7)
    {
        if (strcmp(gmail, user.gmail) == 0 &&
            strcmp(password, user.password) == 0)
        {
            found = 1;

            printf("\nGmail and Password are correct.\n");
            printf("Enter new information:\n\n");

            printf("Enter New Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            removeNewline(user.name);

            printf("Enter New Password: ");
            fgets(user.password, sizeof(user.password), stdin);
            removeNewline(user.password);

            printf("Enter New Address: ");
            fgets(user.address, sizeof(user.address), stdin);
            removeNewline(user.address);

            printf("Enter New Phone Number: ");
            fgets(user.phone, sizeof(user.phone), stdin);
            removeNewline(user.phone);

            printf("Enter New Age: ");

            while (scanf("%d", &user.age) != 1 ||
                   user.age <= 0)
            {
                clearInputBuffer();
                printf("Invalid Age! Enter again: ");
            }

            clearInputBuffer();

            printf("Enter New Gender: ");
            fgets(user.gender, sizeof(user.gender), stdin);
            removeNewline(user.gender);
        }

        fprintf(tempFile,
                "%s|%s|%s|%s|%s|%d|%s\n",
                user.name,
                user.gmail,
                user.password,
                user.address,
                user.phone,
                user.age,
                user.gender);
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove(USER_FILE);
        rename(TEMP_FILE, USER_FILE);

        printf("\n============================================\n");
        printf("   INFORMATION UPDATED SUCCESSFULLY!\n");
        printf("============================================\n");
    }
    else
    {
        remove(TEMP_FILE);

        printf("\nInvalid Gmail or Password!\n");
        printf("Information was not updated.\n");
    }
}


/* ================================
   DELETE MY INFORMATION
   ================================ */

void deleteMyInformation(void)
{
    char gmail[100];
    char password[100];
    char confirm;

    struct User user;

    FILE *file;
    FILE *tempFile;

    int found = 0;

    printf("\n");
    printf("============================================\n");
    printf("           DELETE MY INFORMATION\n");
    printf("============================================\n");

    printf("Enter your Gmail: ");
    fgets(gmail, sizeof(gmail), stdin);
    removeNewline(gmail);

    printf("Enter your Password: ");
    fgets(password, sizeof(password), stdin);
    removeNewline(password);

    file = fopen(USER_FILE, "r");

    if (file == NULL)
    {
        printf("\nNo user information found.\n");
        return;
    }

    tempFile = fopen(TEMP_FILE, "w");

    if (tempFile == NULL)
    {
        printf("\nUnable to create temporary file!\n");
        fclose(file);
        return;
    }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name,
                  user.gmail,
                  user.password,
                  user.address,
                  user.phone,
                  &user.age,
                  user.gender) == 7)
    {
        if (strcmp(gmail, user.gmail) == 0 &&
            strcmp(password, user.password) == 0)
        {
            found = 1;

            continue;
        }

        fprintf(tempFile,
                "%s|%s|%s|%s|%s|%d|%s\n",
                user.name,
                user.gmail,
                user.password,
                user.address,
                user.phone,
                user.age,
                user.gender);
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        printf("\nGmail and Password are correct.\n");

        printf("Are you sure you want to delete your information?\n");
        printf("Enter Y for Yes or N for No: ");

        scanf(" %c", &confirm);
        clearInputBuffer();

        if (confirm == 'Y' || confirm == 'y')
        {
            remove(USER_FILE);
            rename(TEMP_FILE, USER_FILE);

            printf("\n");
            printf("============================================\n");
            printf(" YOUR INFORMATION DELETED SUCCESSFULLY!\n");
            printf("============================================\n");
        }
        else
        {
            remove(TEMP_FILE);

            printf("\nDelete operation cancelled.\n");
        }
    }
    else
    {
        remove(TEMP_FILE);

        printf("\n");
        printf("============================================\n");
        printf("     INVALID GMAIL OR PASSWORD!\n");
        printf("            ACCESS DENIED\n");
        printf("============================================\n");

        printf("Your information was NOT deleted.\n");
    }
}


/* ================================
   USER PANEL
   ================================ */

void userPanel(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("============================================\n");
        printf("               USER PANEL\n");
        printf("============================================\n");

        printf("1. Register New User\n");
        printf("2. View My Information\n");
        printf("3. Update My Information\n");
        printf("4. Delete My Information\n");
        printf("5. Back to Main Menu\n");

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
                registerUser();
                break;

            case 2:
                viewMyInformation();
                break;

            case 3:
                updateMyInformation();
                break;

            case 4:
                deleteMyInformation();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* ================================
   MAIN FUNCTION
   ================================ */

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("============================================\n");
        printf("       DISTRICT MANAGEMENT SYSTEM\n");
        printf("============================================\n");

        printf("1. User Panel\n");
        printf("2. Exit\n");

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
                printf("\nThank you for using the system!\n");
                return 0;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}