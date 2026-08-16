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

/* ================================================
   ADD MALL
   ================================================ */

void addMall(void)
{
    struct Mall mall, temp;
    FILE *file;

    printf("\n+======================================================+\n");
    printf("|                  ADD SHOPPING MALL                   |\n");
    printf("+======================================================+\n");

    inputString(mall.name,     100, "Mall Name              : ");
    inputString(mall.location, 100, "Location               : ");
    inputString(mall.owner,    100, "Owner Name             : ");

    while (1) {
        inputString(mall.gmail, 100, "Owner Gmail (@/.com)   : ");
        if (isValidEmail(mall.gmail)) break;
        printf("Invalid email format! Try again.\n");
    }

    /* Check duplicate gmail in malls.txt */
    file = fopen(FILE_NAME, "r");
    if (file)
    {
        while (fscanf(file,
                      "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                      "%f|%d|%f|%d|%f|%d\n",
                      temp.name, temp.location, temp.owner,
                      temp.gmail, temp.password,
                      &temp.variety_rating,  &temp.variety_rating_count,
                      &temp.clean_rating,    &temp.clean_rating_count,
                      &temp.security_rating, &temp.security_rating_count) == 11)
        {
            if (strcmp(mall.gmail, temp.gmail) == 0)
            {
                fclose(file);
                printf("\nThis Gmail is already registered! Please use a different Gmail.\n");
                return;
            }
        }
        fclose(file);
    }

    while (1) {
        inputString(mall.password, 100, "Owner Password (min 6) : ");
        if (isValidPassword(mall.password)) break;
        printf("Password too short! Try again.\n");
    }

    mall.variety_rating        = 0.0f;
    mall.variety_rating_count  = 0;
    mall.clean_rating          = 0.0f;
    mall.clean_rating_count    = 0;
    mall.security_rating       = 0.0f;
    mall.security_rating_count = 0;

    file = fopen(FILE_NAME, "a");
    if (!file) { printf("\nError: Cannot open malls.txt!\n"); return; }

    fprintf(file, "%s|%s|%s|%s|%s|%.2f|%d|%.2f|%d|%.2f|%d\n",
            mall.name, mall.location, mall.owner,
            mall.gmail, mall.password,
            mall.variety_rating,  mall.variety_rating_count,
            mall.clean_rating,    mall.clean_rating_count,
            mall.security_rating, mall.security_rating_count);

    fclose(file);
    printf("\n+======================================================+\n");
    printf("|         SHOPPING MALL ADDED SUCCESSFULLY!            |\n");
    printf("+======================================================+\n");
}

/* ================================================
   VIEW MALL
   ================================================ */

void viewMall(void)
{
    struct Mall mall;
    FILE *file;
    int count = 0;

    file = fopen(FILE_NAME, "r");
    if (!file) { printf("\nNo shopping mall data found.\n"); return; }

    printf("\n+======================================================+\n");
    printf("|                  SHOPPING MALL LIST                  |\n");
    printf("+======================================================+\n");

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                  "%f|%d|%f|%d|%f|%d\n",
                  mall.name, mall.location, mall.owner,
                  mall.gmail, mall.password,
                  &mall.variety_rating,  &mall.variety_rating_count,
                  &mall.clean_rating,    &mall.clean_rating_count,
                  &mall.security_rating, &mall.security_rating_count) == 11)
    {
        count++;
        printf("\n  Mall #%d\n", count);
        printf("  Name     : %s\n", mall.name);
        printf("  Location : %s\n", mall.location);
        printf("  Owner    : %s\n", mall.owner);
        printf("  Ratings:\n");

        if (mall.variety_rating_count > 0)
            printf("    Shop Variety : %.1f Stars (%d reviews)\n",
                   mall.variety_rating, mall.variety_rating_count);
        else
            printf("    Shop Variety : No ratings yet\n");

        if (mall.clean_rating_count > 0)
            printf("    Cleanliness  : %.1f Stars (%d reviews)\n",
                   mall.clean_rating, mall.clean_rating_count);
        else
            printf("    Cleanliness  : No ratings yet\n");

        if (mall.security_rating_count > 0)
            printf("    Security     : %.1f Stars (%d reviews)\n",
                   mall.security_rating, mall.security_rating_count);
        else
            printf("    Security     : No ratings yet\n");

        /* feedbacks */
        FILE *fb = fopen(FEEDBACK_FILE, "r");
        if (fb)
        {
            char fb_mall[100], fb_user[100], fb_msg[200];
            int  fb_shown = 0;
            while (fscanf(fb, "%99[^|]|%99[^|]|%199[^\n]\n",
                          fb_mall, fb_user, fb_msg) == 3)
            {
                if (strcasecmp(fb_mall, mall.name) == 0)
                {
                    if (!fb_shown) printf("  Feedbacks:\n");
                    printf("    - %s: \"%s\"\n", fb_user, fb_msg);
                    fb_shown = 1;
                }
            }
            fclose(fb);
        }
        printf("+------------------------------------------------------+\n");
    }

    fclose(file);
    if (count == 0) printf("  No shopping mall data found.\n");
}

/* ================================================
   UPDATE MALL
   ================================================ */

void updateMall(void)
{
    char gmail[100], password[100], newGmail[100];
    struct Mall mall;
    FILE *file, *temp;
    int found = 0, choice;

    printf("\n+======================================================+\n");
    printf("|                 UPDATE SHOPPING MALL                 |\n");
    printf("+======================================================+\n");

    inputString(gmail,    100, "Enter Owner Gmail    : ");
    inputString(password, 100, "Enter Owner Password : ");

    file = fopen(FILE_NAME, "r");
    if (!file) { printf("\nNo shopping mall data found.\n"); return; }

    temp = fopen(TEMP_FILE, "w");
    if (!temp) { fclose(file); printf("\nTemp file error.\n"); return; }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                  "%f|%d|%f|%d|%f|%d\n",
                  mall.name, mall.location, mall.owner,
                  mall.gmail, mall.password,
                  &mall.variety_rating,  &mall.variety_rating_count,
                  &mall.clean_rating,    &mall.clean_rating_count,
                  &mall.security_rating, &mall.security_rating_count) == 11)
    {
        if (strcmp(gmail, mall.gmail) == 0 &&
            strcmp(password, mall.password) == 0)
        {
            found = 1;
            printf("\nLogin successful. Enter new information:\n\n");

            inputString(mall.name,     100, "New Mall Name              : ");
            inputString(mall.location, 100, "New Location               : ");
            inputString(mall.owner,    100, "New Owner Name             : ");

            printf("\nDo you want to update Owner Gmail?\n");
            printf("  1. Yes\n  2. No\n");
            choice = readInt("Enter Choice: ", 1, 2);

            if (choice == 1)
            {
                while (1) {
                    inputString(newGmail, 100, "New Owner Gmail (@/.com) : ");
                    if (isValidEmail(newGmail)) break;
                    printf("Invalid email format! Try again.\n");
                }
                strncpy(mall.gmail, newGmail, 99);
                mall.gmail[99] = '\0';
            }

            while (1) {
                inputString(mall.password, 100, "New Password (min 6)       : ");
                if (isValidPassword(mall.password)) break;
                printf("Password too short! Try again.\n");
            }
        }

        fprintf(temp, "%s|%s|%s|%s|%s|%.2f|%d|%.2f|%d|%.2f|%d\n",
                mall.name, mall.location, mall.owner,
                mall.gmail, mall.password,
                mall.variety_rating,  mall.variety_rating_count,
                mall.clean_rating,    mall.clean_rating_count,
                mall.security_rating, mall.security_rating_count);
    }

    fclose(file);
    fclose(temp);

    if (found)
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);
        printf("\n+======================================================+\n");
        printf("|        SHOPPING MALL UPDATED SUCCESSFULLY!           |\n");
        printf("+======================================================+\n");
    }
    else
    {
        remove(TEMP_FILE);
        printf("\nInvalid Gmail or Password!\n");
    }
}
