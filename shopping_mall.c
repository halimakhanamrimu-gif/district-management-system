#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FILE_NAME     "malls.txt"
#define TEMP_FILE     "temp_mall.txt"
#define USER_FILE     "users.txt"
#define FEEDBACK_FILE "mall_feedbacks.txt"

/* ================================================
   STRUCTURE
   ================================================ */

struct Mall
{
    char name[100];
    char location[100];
    char owner[100];
    char gmail[100];
    char password[100];
    float variety_rating;
    int   variety_rating_count;
    float clean_rating;
    int   clean_rating_count;
    float security_rating;
    int   security_rating_count;
};

/* ================================================
   PROTOTYPES
   ================================================ */

void addMall(void);
void viewMall(void);
void updateMall(void);
void deleteMall(void);
void mallOwner(void);
void buyerPanel(void);
void mallMenu(void);

static void   inputString(char str[], int size, const char *message);
static int    readInt(const char *prompt, int min, int max);
static void   registerBuyer(void);
static int    checkGlobalUser(const char *gmail, const char *password);
static int    isValidEmail(const char *email);
static int    isValidPassword(const char *password);

/* ================================================
   HELPERS
   ================================================ */

static void inputString(char str[], int size, const char *message)
{
    printf("%s", message);
    if (fgets(str, size, stdin) != NULL)
        str[strcspn(str, "\n")] = '\0';
}

/* Safe integer input — no leftover in buffer */
static int readInt(const char *prompt, int min, int max)
{
    int val;
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

static int isValidEmail(const char *email)
{
    return (strchr(email, '@') != NULL && strstr(email, ".com") != NULL) ? 1 : 0;
}

static int isValidPassword(const char *password)
{
    return (strlen(password) >= 6) ? 1 : 0;
}

/* Check buyer against users.txt */
static int checkGlobalUser(const char *gmail, const char *password)
{
    FILE *file = fopen(USER_FILE, "r");
    if (!file) return 0;

    char u_name[100], u_gmail[100], u_pass[100], u_addr[150];
    char u_phone[20], u_nid[20], u_gender[20];
    int  u_age;

    /* Format: name|gmail|pass|addr|phone|nid|age|gender */
    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%19[^|]|%d|%19[^\n]\n",
                  u_name, u_gmail, u_pass, u_addr,
                  u_phone, u_nid, &u_age, u_gender) == 8)
    {
        if (strcmp(gmail, u_gmail) == 0 && strcmp(password, u_pass) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/* ================================================
   REGISTER BUYER  (inside buyer panel)
   ================================================ */

static void registerBuyer(void)
{
    char name[100], gmail[100], password[100];
    char address[150], phone[20], nid[20], gender[20];
    int  age;
    FILE *file;

    printf("\n+======================================================+\n");
    printf("|                  USER REGISTRATION                   |\n");
    printf("+======================================================+\n");

    inputString(name,    100, "Enter Name             : ");

    while (1) {
        inputString(gmail, 100, "Enter Gmail (@/.com)   : ");
        if (isValidEmail(gmail)) break;
        printf("Invalid email format! Try again.\n");
    }

    /* duplicate check */
    file = fopen(USER_FILE, "r");
    if (file)
    {
        char u_name[100], u_gmail[100], u_pass[100], u_addr[150];
        char u_phone[20], u_nid[20], u_gender[20];
        int  u_age;
        while (fscanf(file,
                      "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%19[^|]|%d|%19[^\n]\n",
                      u_name, u_gmail, u_pass, u_addr,
                      u_phone, u_nid, &u_age, u_gender) == 8)
        {
            if (strcmp(gmail, u_gmail) == 0)
            {
                fclose(file);
                printf("\nThis Gmail is already registered! Please login.\n");
                return;
            }
        }
        fclose(file);
    }

    while (1) {
        inputString(password, 100, "Enter Password (min 6) : ");
        if (isValidPassword(password)) break;
        printf("Password too short! Try again.\n");
    }

    inputString(address, 150, "Enter Address          : ");
    inputString(phone,    20, "Enter Phone Number     : ");
    inputString(nid,      20, "Enter NID Number       : ");

    age = readInt("Enter Age              : ", 1, 120);

    inputString(gender,   20, "Enter Gender           : ");

    file = fopen(USER_FILE, "a");
    if (!file) { printf("\nError: Cannot open users.txt!\n"); return; }

    fprintf(file, "%s|%s|%s|%s|%s|%s|%d|%s\n",
            name, gmail, password, address, phone, nid, age, gender);
    fclose(file);

    printf("\n+======================================================+\n");
    printf("|         REGISTRATION SUCCESSFUL!                     |\n");
    printf("|   You can now login with your Gmail & Password.      |\n");
    printf("+======================================================+\n");
}
