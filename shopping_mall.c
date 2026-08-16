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

