#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define USER_FILE     "users.txt"
#define TEMP_FILE     "temp_users.txt"
#define BOOKING_FILE  "bookings.txt"
#define TEMP_BOOKING  "temp_bookings.txt"
#define ADMIN_PASS    "admin1234"

/* ================================================
   HOSPITAL & DOCTOR DATA  (static list)
   ================================================ */

#define HOSPITAL_COUNT 4
#define MAX_DOCTORS    5

static const char *hospitals[HOSPITAL_COUNT] = {
    "Dhaka Medical College Hospital",
    "Sir Salimullah Medical College Hospital",
    "Shaheed Suhrawardy Medical College Hospital",
    "National Institute of Cancer Research Hospital"
};

static const char *doctors[HOSPITAL_COUNT][MAX_DOCTORS] = {
    {
        "Dr. Rahim Uddin   (Medicine)",
        "Dr. Nasrin Akter  (Gynecology)",
        "Dr. Kamal Hossain (Surgery)",
        "Dr. Farida Begum  (Pediatrics)",
        "Dr. Tariq Islam   (Cardiology)"
    },
    {
        "Dr. Shafiqul Islam  (Medicine)",
        "Dr. Rokeya Khanam   (Gynecology)",
        "Dr. Aminul Haq      (Orthopedics)",
        "Dr. Sultana Parvin  (ENT)",
        "Dr. Mahbubur Rahman (Neurology)"
    },
    {
        "Dr. Zahirul Hasan   (Surgery)",
        "Dr. Monira Begum    (Pediatrics)",
        "Dr. Rezaul Karim    (Cardiology)",
        "Dr. Sharmin Akter   (Dermatology)",
        "Dr. Nurul Islam     (Urology)"
    },
    {
        "Dr. Anwarul Azim    (Oncology)",
        "Dr. Lovely Khanam   (Radiation Therapy)",
        "Dr. Masud Parvez    (Surgical Oncology)",
        "Dr. Nasima Sultana  (Medical Oncology)",
        "Dr. Khairul Bashar  (Palliative Care)"
    }
};


/* ================================================
   STRUCTURES
   ================================================ */

struct User
{
    char name[100];
    char gmail[100];
    char password[100];
    char address[150];
    char phone[20];
    char nid[20];
    int  age;
    char gender[20];
};

struct Booking
{
    char userGmail[100];
    char userNID[20];
    char hospitalName[150];
    char doctorName[150];
    char bookingDate[30];
};



/* ================================================
   FUNCTION PROTOTYPES
   ================================================ */

void userPanel(void);
void patientPanel(void);
void adminPanel(void);
void registerUser(void);
void viewMyInformation(void);
void updateMyInformation(void);
void deleteMyInformation(void);
void bookDoctor(void);
void viewMyBookings(void);
void cancelBooking(void);
void viewAllUsers(void);
void viewAllBookings(void);

static void inputString(char str[], int size, const char *message);
static int  readInt(const char *prompt, int min, int max);
static int  isValidEmail(const char *email);
static int  isValidPassword(const char *password);
int  gmailExists(const char gmail[]);
int  findUser(const char gmail[], const char password[], struct User *foundUser);


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
        printf("Invalid input! Enter a number between %d and %d: ", min, max);
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

int gmailExists(const char gmail[])
{
    FILE *file = fopen(USER_FILE, "r");
    if (!file) return 0;

    struct User user;
    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name, user.gmail, user.password,
                  user.address, user.phone, user.nid,
                  &user.age, user.gender) == 8)
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

int findUser(const char gmail[], const char password[], struct User *foundUser)
{
    FILE *file = fopen(USER_FILE, "r");
    if (!file) return 0;

    struct User user;
    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name, user.gmail, user.password,
                  user.address, user.phone, user.nid,
                  &user.age, user.gender) == 8)
    {
        if (strcmp(gmail, user.gmail) == 0 && strcmp(password, user.password) == 0)
        {
            *foundUser = user;
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}


/* ================================================
   REGISTER NEW USER
   ================================================ */

void registerUser(void)
{
    struct User user;
    FILE *file;

    printf("\n+======================================================+\n");
    printf("|                  USER REGISTRATION                   |\n");
    printf("+======================================================+\n");

    inputString(user.name, sizeof(user.name), "Enter Full Name        : ");

    /* Email validation with retry */
    while (1) {
        inputString(user.gmail, sizeof(user.gmail), "Enter Gmail (@/.com)   : ");
        if (isValidEmail(user.gmail)) break;
        printf("Invalid email format! Try again.\n");
    }

    if (gmailExists(user.gmail))
    {
        printf("\nThis Gmail is already registered! Please login.\n");
        return;
    }

    /* Password validation with retry */
    while (1) {
        inputString(user.password, sizeof(user.password), "Enter Password (min 6) : ");
        if (isValidPassword(user.password)) break;
        printf("Password too short! Try again.\n");
    }

    inputString(user.address, sizeof(user.address), "Enter Address          : ");
    inputString(user.phone,   sizeof(user.phone),   "Enter Phone Number     : ");
    inputString(user.nid,     sizeof(user.nid),     "Enter NID Number       : ");

    user.age = readInt("Enter Age              : ", 1, 120);

    inputString(user.gender, sizeof(user.gender), "Enter Gender           : ");

    file = fopen(USER_FILE, "a");
    if (!file) { printf("\nERROR: Unable to open users.txt!\n"); return; }

    fprintf(file, "%s|%s|%s|%s|%s|%s|%d|%s\n",
            user.name, user.gmail, user.password,
            user.address, user.phone, user.nid,
            user.age, user.gender);

    fclose(file);

    printf("\n+======================================================+\n");
    printf("|           REGISTRATION SUCCESSFUL!                   |\n");
    printf("|    You can now login with your Gmail & Password.     |\n");
    printf("+======================================================+\n");
}


/* ================================================
   VIEW MY INFORMATION
   ================================================ */

void viewMyInformation(void)
{
    char gmail[100], password[100];
    struct User user;

    printf("\n+======================================================+\n");
    printf("|               VERIFY YOUR IDENTITY                   |\n");
    printf("+======================================================+\n");

    inputString(gmail,    sizeof(gmail),    "Enter your Gmail    : ");
    inputString(password, sizeof(password), "Enter your Password : ");

    if (findUser(gmail, password, &user))
    {
        printf("\n+======================================================+\n");
        printf("|                  MY INFORMATION                      |\n");
        printf("+======================================================+\n");
        printf("  Name    : %s\n", user.name);
        printf("  Gmail   : %s\n", user.gmail);
        printf("  Address : %s\n", user.address);
        printf("  Phone   : %s\n", user.phone);
        printf("  NID     : %s\n", user.nid);
        printf("  Age     : %d\n", user.age);
        printf("  Gender  : %s\n", user.gender);
        printf("+------------------------------------------------------+\n");
    }
    else
    {
        printf("\n+======================================================+\n");
        printf("|       INVALID GMAIL OR PASSWORD! ACCESS DENIED       |\n");
        printf("+======================================================+\n");
    }
}


/* ================================================
   UPDATE MY INFORMATION
   ================================================ */

void updateMyInformation(void)
{
    char gmail[100], password[100];
    struct User user;
    FILE *file, *tempFile;
    int found = 0;

    printf("\n+======================================================+\n");
    printf("|              UPDATE MY INFORMATION                   |\n");
    printf("+======================================================+\n");

    inputString(gmail,    sizeof(gmail),    "Enter your Gmail    : ");
    inputString(password, sizeof(password), "Enter your Password : ");

    file = fopen(USER_FILE, "r");
    if (!file) { printf("\nNo user information found.\n"); return; }

    tempFile = fopen(TEMP_FILE, "w");
    if (!tempFile) { fclose(file); printf("\nTemp file error!\n"); return; }

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name, user.gmail, user.password,
                  user.address, user.phone, user.nid,
                  &user.age, user.gender) == 8)
    {
        if (strcmp(gmail, user.gmail) == 0 && strcmp(password, user.password) == 0)
        {
            found = 1;
            printf("\nCredentials verified. Enter new information:\n\n");

            inputString(user.name, sizeof(user.name), "New Full Name          : ");

            /* Password validation with retry */
            while (1) {
                inputString(user.password, sizeof(user.password), "New Password (min 6)   : ");
                if (isValidPassword(user.password)) break;
                printf("Password too short! Try again.\n");
            }

            inputString(user.address, sizeof(user.address), "New Address            : ");
            inputString(user.phone,   sizeof(user.phone),   "New Phone Number       : ");
            inputString(user.nid,     sizeof(user.nid),     "New NID Number         : ");

            user.age = readInt("New Age                : ", 1, 120);

            inputString(user.gender, sizeof(user.gender), "New Gender             : ");
        }

        fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%d|%s\n",
                user.name, user.gmail, user.password,
                user.address, user.phone, user.nid,
                user.age, user.gender);
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove(USER_FILE);
        rename(TEMP_FILE, USER_FILE);
        printf("\n+======================================================+\n");
        printf("|        INFORMATION UPDATED SUCCESSFULLY!             |\n");
        printf("+======================================================+\n");
    }
    else
    {
        remove(TEMP_FILE);
        printf("\nInvalid Gmail or Password! Information not updated.\n");
    }
}

