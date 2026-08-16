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
