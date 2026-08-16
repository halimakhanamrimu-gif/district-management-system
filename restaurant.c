#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FILE_NAME     "restaurants.txt"
#define TEMP_FILE     "temp_restaurant.txt"
#define USER_FILE     "users.txt"
#define FEEDBACK_FILE "restaurant_feedbacks.txt"

/* ================================================
   STRUCTURE
   ================================================ */

struct Restaurant
{
    char name[100];
    char location[100];
    char owner[100];
    char gmail[100];
    char password[100];
    float waiter_rating;
    int   waiter_rating_count;
    float food_rating;
    int   food_rating_count;
    float env_rating;
    int   env_rating_count;
};

/* ================================================
   PROTOTYPES
   ================================================ */

void addRestaurant(void);
void viewRestaurant(void);
void updateRestaurant(void);
void deleteRestaurant(void);
void restaurantOwner(void);
void customerPanel(void);
void restaurantMenu(void);

static void  inputString(char str[], int size, const char *message);
static int   readInt(const char *prompt, int min, int max);
static void  registerCustomer(void);
static int   checkGlobalUser(const char *gmail, const char *password);
static int   isValidEmail(const char *email);
static int   isValidPassword(const char *password);

/* ================================================
   HELPERS
   ================================================ */

static void inputString(char str[], int size, const char *message)
{
    printf("%s", message);
    if (fgets(str, size, stdin) != NULL)
        str[strcspn(str, "\n")] = '\0';
}

static int readInt(const char *prompt, int min, int max)
{
    int  val;
    char line[32];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) continue;
        if (sscanf(line, "%d", &val) == 1 && val >= min && val <= max)
            return val;
        printf("Invalid! Enter a number between %d and %d: ", min, max);
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

/* ================================================
   CHECK GLOBAL USER  — 8-field format
   name|gmail|pass|addr|phone|nid|age|gender
   ================================================ */

static int checkGlobalUser(const char *gmail, const char *password)
{
    FILE *file = fopen(USER_FILE, "r");
    if (!file) return 0;

    char u_name[100], u_gmail[100], u_pass[100], u_addr[150];
    char u_phone[20], u_nid[20], u_gender[20];
    int  u_age;

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
   REGISTER CUSTOMER  — inside customer panel
   Saves 8-field format with NID
   ================================================ */

static void registerCustomer(void)
{
    char name[100], gmail[100], password[100];
    char address[150], phone[20], nid[20], gender[20];
    int  age;
    FILE *file;

    printf("\n+======================================================+\n");
    printf("|                  USER REGISTRATION                   |\n");
    printf("+======================================================+\n");

    inputString(name, 100, "Enter Full Name        : ");

    while (1) {
        inputString(gmail, 100, "Enter Gmail (@/.com)   : ");
        if (isValidEmail(gmail)) break;
        printf("Invalid email format! Try again.\n");
    }

    /* duplicate gmail check — reads 8-field */
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

    inputString(gender, 20, "Enter Gender           : ");

    file = fopen(USER_FILE, "a");
    if (!file) { printf("\nError: Cannot open users.txt!\n"); return; }

    /* 8-field: name|gmail|pass|addr|phone|nid|age|gender */
    fprintf(file, "%s|%s|%s|%s|%s|%s|%d|%s\n",
            name, gmail, password, address, phone, nid, age, gender);
    fclose(file);

    printf("\n+======================================================+\n");
    printf("|           REGISTRATION SUCCESSFUL!                   |\n");
    printf("|    You can now login with your Gmail & Password.     |\n");
    printf("+======================================================+\n");
}

/* ================================================
   ADD RESTAURANT
   ================================================ */

void addRestaurant(void)
{
    struct Restaurant r, temp;
    FILE *file;

    printf("\n+======================================================+\n");
    printf("|                    ADD RESTAURANT                    |\n");
    printf("+======================================================+\n");

    inputString(r.name,     100, "Restaurant Name              : ");
    inputString(r.location, 100, "Location                     : ");
    inputString(r.owner,    100, "Owner Name                   : ");

    while (1) {
        inputString(r.gmail, 100, "Owner Gmail (@/.com)         : ");
        if (isValidEmail(r.gmail)) break;
        printf("Invalid email format! Try again.\n");
    }

    /* Check duplicate gmail in restaurants.txt */
    file = fopen(FILE_NAME, "r");
    if (file)
    {
        while (fscanf(file,
                      "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                      "%f|%d|%f|%d|%f|%d\n",
                      temp.name, temp.location, temp.owner, temp.gmail, temp.password,
                      &temp.waiter_rating, &temp.waiter_rating_count,
                      &temp.food_rating,   &temp.food_rating_count,
                      &temp.env_rating,    &temp.env_rating_count) == 11)
        {
            if (strcmp(r.gmail, temp.gmail) == 0)
            {
                fclose(file);
                printf("\nThis Gmail is already registered! Please use a different Gmail.\n");
                return;
            }
        }
        fclose(file);
    }

    while (1) {
        inputString(r.password, 100, "Owner Password (min 6 chars) : ");
        if (isValidPassword(r.password)) break;
        printf("Password too short! Try again.\n");
    }

    r.waiter_rating = 0.0f; r.waiter_rating_count = 0;
    r.food_rating   = 0.0f; r.food_rating_count   = 0;
    r.env_rating    = 0.0f; r.env_rating_count    = 0;

    file = fopen(FILE_NAME, "a");
    if (!file) { printf("\nError: Cannot open restaurants.txt!\n"); return; }

    fprintf(file, "%s|%s|%s|%s|%s|%.2f|%d|%.2f|%d|%.2f|%d\n",
            r.name, r.location, r.owner, r.gmail, r.password,
            r.waiter_rating, r.waiter_rating_count,
            r.food_rating,   r.food_rating_count,
            r.env_rating,    r.env_rating_count);
    fclose(file);

    printf("\n+======================================================+\n");
    printf("|           RESTAURANT ADDED SUCCESSFULLY!             |\n");
    printf("+======================================================+\n");
}
