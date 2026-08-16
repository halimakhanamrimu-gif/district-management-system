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
    char nid[20];       /* National ID */
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

/* panels */
void userPanel(void);
void patientPanel(void);
void adminPanel(void);

/* patient actions */
void registerUser(void);
void viewMyInformation(void);
void updateMyInformation(void);
void deleteMyInformation(void);
void bookDoctor(void);
void viewMyBookings(void);
void cancelBooking(void);

/* admin actions */
void viewAllUsers(void);
void viewAllBookings(void);

/* helpers */
void removeNewline(char str[]);
void clearInputBuffer(void);
static void   inputString(char str[], int size, const char *message);
static int    readInt(const char *prompt, int min, int max);
int  gmailExists(const char gmail[]);
int  findUser(const char gmail[], const char password[], struct User *foundUser);


/* ================================================
   HELPERS
   ================================================ */

void removeNewline(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}

void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void inputString(char str[], int size, const char *message)
{
    printf("%s", message);
    if (fgets(str, size, stdin) != NULL)
        str[strcspn(str, "\n")] = '\0';
}

/* Safe integer input — reads whole line, no leftover in buffer */
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


/* ================================================
   CHECK GMAIL EXISTS
   ================================================ */

int gmailExists(const char gmail[])
{
    FILE *file;
    struct User user;

    file = fopen(USER_FILE, "r");
    if (file == NULL) return 0;

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

