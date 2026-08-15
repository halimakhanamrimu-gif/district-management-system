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
    struct Restaurant r;
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

/* ================================================
   VIEW RESTAURANT
   ================================================ */

void viewRestaurant(void)
{
    struct Restaurant r;
    FILE *file;
    int count = 0;

    file = fopen(FILE_NAME, "r");
    if (!file) { printf("\nNo restaurant data found.\n"); return; }

    printf("\n+======================================================+\n");
    printf("|                   RESTAURANT LIST                    |\n");
    printf("+======================================================+\n");

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                  "%f|%d|%f|%d|%f|%d\n",
                  r.name, r.location, r.owner, r.gmail, r.password,
                  &r.waiter_rating, &r.waiter_rating_count,
                  &r.food_rating,   &r.food_rating_count,
                  &r.env_rating,    &r.env_rating_count) == 11)
    {
        count++;
        printf("\n  Restaurant #%d\n", count);
        printf("  Name     : %s\n", r.name);
        printf("  Location : %s\n", r.location);
        printf("  Owner    : %s\n", r.owner);
        printf("  Ratings:\n");

        if (r.waiter_rating_count > 0)
            printf("    Waiter Behavior : %.1f Stars (%d reviews)\n",
                   r.waiter_rating, r.waiter_rating_count);
        else
            printf("    Waiter Behavior : No ratings yet\n");

        if (r.food_rating_count > 0)
            printf("    Food Quality    : %.1f Stars (%d reviews)\n",
                   r.food_rating, r.food_rating_count);
        else
            printf("    Food Quality    : No ratings yet\n");

        if (r.env_rating_count > 0)
            printf("    Environment     : %.1f Stars (%d reviews)\n",
                   r.env_rating, r.env_rating_count);
        else
            printf("    Environment     : No ratings yet\n");

        /* feedbacks */
        FILE *fb = fopen(FEEDBACK_FILE, "r");
        if (fb)
        {
            char fb_rname[100], fb_user[100], fb_msg[200];
            int  fb_shown = 0;
            while (fscanf(fb, "%99[^|]|%99[^|]|%199[^\n]\n",
                          fb_rname, fb_user, fb_msg) == 3)
            {
                if (strcasecmp(fb_rname, r.name) == 0)
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
    if (count == 0) printf("  No restaurant data found.\n");
}

/* ================================================
   UPDATE RESTAURANT
   ================================================ */

void updateRestaurant(void)
{
    char gmail[100], password[100];
    struct Restaurant r;
    FILE *file, *temp;
    int found = 0;

    printf("\n+======================================================+\n");
    printf("|                  UPDATE RESTAURANT                   |\n");
    printf("+======================================================+\n");

    inputString(gmail,    100, "Enter Owner Gmail    : ");
    inputString(password, 100, "Enter Owner Password : ");

    file = fopen(FILE_NAME, "r");
    if (!file) { printf("\nNo restaurant data found.\n"); return; }

    temp = fopen(TEMP_FILE, "w");
    if (!temp) { fclose(file); printf("\nTemp file error.\n"); return; }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                  "%f|%d|%f|%d|%f|%d\n",
                  r.name, r.location, r.owner, r.gmail, r.password,
                  &r.waiter_rating, &r.waiter_rating_count,
                  &r.food_rating,   &r.food_rating_count,
                  &r.env_rating,    &r.env_rating_count) == 11)
    {
        if (strcmp(gmail, r.gmail) == 0 && strcmp(password, r.password) == 0)
        {
            found = 1;
            printf("\nLogin successful. Enter new information:\n\n");

            inputString(r.name,     100, "New Restaurant Name  : ");
            inputString(r.location, 100, "New Location         : ");
            inputString(r.owner,    100, "New Owner Name       : ");

            while (1) {
                inputString(r.password, 100, "New Password (min 6) : ");
                if (isValidPassword(r.password)) break;
                printf("Password too short! Try again.\n");
            }
        }

        fprintf(temp, "%s|%s|%s|%s|%s|%.2f|%d|%.2f|%d|%.2f|%d\n",
                r.name, r.location, r.owner, r.gmail, r.password,
                r.waiter_rating, r.waiter_rating_count,
                r.food_rating,   r.food_rating_count,
                r.env_rating,    r.env_rating_count);
    }

    fclose(file);
    fclose(temp);

    if (found)
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);
        printf("\n+======================================================+\n");
        printf("|        RESTAURANT UPDATED SUCCESSFULLY!              |\n");
        printf("+======================================================+\n");
    }
    else
    {
        remove(TEMP_FILE);
        printf("\nInvalid Gmail or Password!\n");
    }
}

/* ================================================
   DELETE RESTAURANT
   ================================================ */

void deleteRestaurant(void)
{
    char gmail[100], password[100], confirm[8];
    struct Restaurant r;
    FILE *file, *temp;
    int found = 0;

    printf("\n+======================================================+\n");
    printf("|                  DELETE RESTAURANT                   |\n");
    printf("+======================================================+\n");

    inputString(gmail,    100, "Enter Owner Gmail    : ");
    inputString(password, 100, "Enter Owner Password : ");

    file = fopen(FILE_NAME, "r");
    if (!file) { printf("\nNo restaurant data found.\n"); return; }

    temp = fopen(TEMP_FILE, "w");
    if (!temp) { fclose(file); printf("\nTemp file error.\n"); return; }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                  "%f|%d|%f|%d|%f|%d\n",
                  r.name, r.location, r.owner, r.gmail, r.password,
                  &r.waiter_rating, &r.waiter_rating_count,
                  &r.food_rating,   &r.food_rating_count,
                  &r.env_rating,    &r.env_rating_count) == 11)
    {
        if (strcmp(gmail, r.gmail) == 0 && strcmp(password, r.password) == 0)
        {
            found = 1;
            continue;
        }

        fprintf(temp, "%s|%s|%s|%s|%s|%.2f|%d|%.2f|%d|%.2f|%d\n",
                r.name, r.location, r.owner, r.gmail, r.password,
                r.waiter_rating, r.waiter_rating_count,
                r.food_rating,   r.food_rating_count,
                r.env_rating,    r.env_rating_count);
    }

    fclose(file);
    fclose(temp);

    if (!found)
    {
        remove(TEMP_FILE);
        printf("\nInvalid Gmail or Password!\n");
        return;
    }

    inputString(confirm, sizeof(confirm),
                "\nAre you sure? Enter Y to confirm / N to cancel: ");

    if (confirm[0] == 'Y' || confirm[0] == 'y')
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);
        printf("\n+======================================================+\n");
        printf("|         RESTAURANT DELETED SUCCESSFULLY!             |\n");
        printf("+======================================================+\n");
    }
    else
    {
        remove(TEMP_FILE);
        printf("\nDelete operation cancelled.\n");
    }
}

/* ================================================
   CUSTOMER PANEL
   ================================================ */

void customerPanel(void)
{
    char userGmail[100], userPassword[100];

    while (1)
    {
        printf("\n+======================================================+\n");
        printf("|                  CUSTOMER SECTION                    |\n");
        printf("+======================================================+\n");
        printf("|  1. Register New Account                             |\n");
        printf("|  2. Login                                            |\n");
        printf("|  3. Back                                             |\n");
        printf("+------------------------------------------------------+\n");

        int preChoice = readInt("Enter Choice: ", 1, 3);

        if (preChoice == 3) return;

        if (preChoice == 1) { registerCustomer(); continue; }

        /* Login */
        printf("\n+======================================================+\n");
        printf("|                   CUSTOMER LOGIN                     |\n");
        printf("+======================================================+\n");

        inputString(userGmail,    100, "Enter your Gmail    : ");
        inputString(userPassword, 100, "Enter your Password : ");

        if (!checkGlobalUser(userGmail, userPassword))
        {
            printf("\nInvalid Gmail or Password! Access Denied.\n");
            continue;
        }
        break;
    }

    printf("\nLogin Successful! Welcome Customer.\n");

    int choice;
    while (1)
    {
        printf("\n+======================================================+\n");
        printf("|                   CUSTOMER PANEL                     |\n");
        printf("+======================================================+\n");
        printf("|  1. View All Restaurants                             |\n");
        printf("|  2. Rate a Restaurant                                |\n");
        printf("|  3. Give Feedback                                    |\n");
        printf("|  4. Logout                                           |\n");
        printf("+------------------------------------------------------+\n");

        choice = readInt("Enter Choice: ", 1, 4);

        switch (choice)
        {
            case 1:
                viewRestaurant();
                break;

            case 2:
            {
                char rName[100];
                struct Restaurant r;
                FILE *file, *temp;
                int found = 0;

                inputString(rName, 100, "Enter Restaurant Name to rate: ");

                file = fopen(FILE_NAME, "r");
                if (!file) { printf("\nNo restaurant data found.\n"); break; }

                temp = fopen(TEMP_FILE, "w");
                if (!temp) { fclose(file); printf("\nTemp file error.\n"); break; }

                while (fscanf(file,
                              "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                              "%f|%d|%f|%d|%f|%d\n",
                              r.name, r.location, r.owner, r.gmail, r.password,
                              &r.waiter_rating, &r.waiter_rating_count,
                              &r.food_rating,   &r.food_rating_count,
                              &r.env_rating,    &r.env_rating_count) == 11)
                {
                    if (strcasecmp(rName, r.name) == 0 && !found)
                    {
                        found = 1;
                        printf("\nRestaurant found! Enter ratings (1-5 stars).\n");

                        int wR = readInt("Waiter Behavior Rating : ", 1, 5);
                        int fR = readInt("Food Quality Rating    : ", 1, 5);
                        int eR = readInt("Environment Rating     : ", 1, 5);

                        r.waiter_rating =
                            (r.waiter_rating * r.waiter_rating_count + wR)
                            / (float)(r.waiter_rating_count + 1);
                        r.waiter_rating_count++;

                        r.food_rating =
                            (r.food_rating * r.food_rating_count + fR)
                            / (float)(r.food_rating_count + 1);
                        r.food_rating_count++;

                        r.env_rating =
                            (r.env_rating * r.env_rating_count + eR)
                            / (float)(r.env_rating_count + 1);
                        r.env_rating_count++;

                        printf("\nThank you! Ratings updated successfully.\n");
                    }

                    fprintf(temp, "%s|%s|%s|%s|%s|%.2f|%d|%.2f|%d|%.2f|%d\n",
                            r.name, r.location, r.owner, r.gmail, r.password,
                            r.waiter_rating, r.waiter_rating_count,
                            r.food_rating,   r.food_rating_count,
                            r.env_rating,    r.env_rating_count);
                }

                fclose(file);
                fclose(temp);

                if (found) { remove(FILE_NAME); rename(TEMP_FILE, FILE_NAME); }
                else       { remove(TEMP_FILE); printf("\nRestaurant '%s' not found!\n", rName); }
                break;
            }

            case 3:
            {
                char rName[100], feedback[200];
                struct Restaurant r;
                int found = 0;
                FILE *file;

                inputString(rName, 100, "Enter Restaurant Name for feedback: ");

                file = fopen(FILE_NAME, "r");
                if (file)
                {
                    while (fscanf(file,
                                  "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|"
                                  "%f|%d|%f|%d|%f|%d\n",
                                  r.name, r.location, r.owner, r.gmail, r.password,
                                  &r.waiter_rating, &r.waiter_rating_count,
                                  &r.food_rating,   &r.food_rating_count,
                                  &r.env_rating,    &r.env_rating_count) == 11)
                    {
                        if (strcasecmp(rName, r.name) == 0) { found = 1; break; }
                    }
                    fclose(file);
                }

                if (found)
                {
                    inputString(feedback, 200, "Enter your feedback message: ");
                    FILE *fb = fopen(FEEDBACK_FILE, "a");
                    if (fb)
                    {
                        fprintf(fb, "%s|%s|%s\n", r.name, userGmail, feedback);
                        fclose(fb);
                        printf("\nThank you! Feedback saved successfully.\n");
                    }
                    else printf("\nError saving feedback!\n");
                }
                else printf("\nRestaurant '%s' not found!\n", rName);
                break;
            }

            case 4:
                return;
        }
    }
}

/* ================================================
   RESTAURANT OWNER PANEL
   ================================================ */

void restaurantOwner(void)
{
    int choice;

    while (1)
    {
        printf("\n+======================================================+\n");
        printf("|               RESTAURANT OWNER PANEL                 |\n");
        printf("+======================================================+\n");
        printf("|  1. Add Restaurant                                   |\n");
        printf("|  2. View Restaurants                                 |\n");
        printf("|  3. Update Restaurant                                |\n");
        printf("|  4. Delete Restaurant                                |\n");
        printf("|  5. Back                                             |\n");
        printf("+------------------------------------------------------+\n");

        choice = readInt("Enter Choice: ", 1, 5);

        switch (choice)
        {
            case 1: addRestaurant();    break;
            case 2: viewRestaurant();   break;
            case 3: updateRestaurant(); break;
            case 4: deleteRestaurant(); break;
            case 5: return;
        }
    }
}

/* ================================================
   RESTAURANT MENU
   ================================================ */

void restaurantMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n+======================================================+\n");
        printf("|                  RESTAURANT MENU                     |\n");
        printf("+======================================================+\n");
        printf("|  1. Restaurant Owner Panel                           |\n");
        printf("|  2. Customer Panel                                   |\n");
        printf("|  3. Back to Main Menu                                |\n");
        printf("+------------------------------------------------------+\n");

        choice = readInt("Enter Choice: ", 1, 3);

        switch (choice)
        {
            case 1: restaurantOwner(); break;
            case 2: customerPanel();   break;
            case 3: return;
        }
    }
}
