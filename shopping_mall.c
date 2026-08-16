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
