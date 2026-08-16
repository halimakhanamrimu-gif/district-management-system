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


/* ================================================
   DELETE MY INFORMATION
   ================================================ */

void deleteMyInformation(void)
{
    char gmail[100], password[100], confirm[8];
    struct User user;
    FILE *file, *tempFile;
    int found = 0;

    printf("\n+======================================================+\n");
    printf("|              DELETE MY INFORMATION                   |\n");
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
            continue;
        }

        fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%d|%s\n",
                user.name, user.gmail, user.password,
                user.address, user.phone, user.nid,
                user.age, user.gender);
    }

    fclose(file);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_FILE);
        printf("\n+======================================================+\n");
        printf("|       INVALID GMAIL OR PASSWORD! ACCESS DENIED       |\n");
        printf("+======================================================+\n");
        return;
    }

    inputString(confirm, sizeof(confirm),
                "\nAre you sure? Enter Y to confirm / N to cancel: ");

    if (confirm[0] == 'Y' || confirm[0] == 'y')
    {
        remove(USER_FILE);
        rename(TEMP_FILE, USER_FILE);
        printf("\n+======================================================+\n");
        printf("|      YOUR INFORMATION DELETED SUCCESSFULLY!          |\n");
        printf("+======================================================+\n");
    }
    else
    {
        remove(TEMP_FILE);
        printf("\nDelete operation cancelled.\n");
    }
}


/* ================================================
   BOOK A DOCTOR
   ================================================ */

void bookDoctor(void)
{
    char gmail[100], password[100];
    struct User user;
    struct Booking booking;
    FILE *file;
    int hChoice, dChoice, i;

    printf("\n+======================================================+\n");
    printf("|                   BOOK A DOCTOR                      |\n");
    printf("+======================================================+\n");

    inputString(gmail,    sizeof(gmail),    "Enter your Gmail    : ");
    inputString(password, sizeof(password), "Enter your Password : ");

    if (!findUser(gmail, password, &user))
    {
        printf("\nInvalid Gmail or Password! Access Denied.\n");
        return;
    }

    printf("\nWelcome, %s!\n", user.name);
    printf("NID on file: %s\n", user.nid);

    printf("\n+------------------------------------------------------+\n");
    printf("|                  SELECT HOSPITAL                     |\n");
    printf("+------------------------------------------------------+\n");
    for (i = 0; i < HOSPITAL_COUNT; i++)
        printf("|  %d. %-48s|\n", i + 1, hospitals[i]);
    printf("+------------------------------------------------------+\n");

    hChoice = readInt("Enter Hospital Choice (1-4): ", 1, HOSPITAL_COUNT);

    strncpy(booking.hospitalName, hospitals[hChoice - 1], sizeof(booking.hospitalName) - 1);
    booking.hospitalName[sizeof(booking.hospitalName) - 1] = '\0';

    printf("\n+------------------------------------------------------+\n");
    printf("|              AVAILABLE DOCTORS                       |\n");
    printf("+------------------------------------------------------+\n");
    for (i = 0; i < MAX_DOCTORS; i++)
        printf("|  %d. %-48s|\n", i + 1, doctors[hChoice - 1][i]);
    printf("+------------------------------------------------------+\n");

    dChoice = readInt("Enter Doctor Choice (1-5): ", 1, MAX_DOCTORS);

    strncpy(booking.doctorName, doctors[hChoice - 1][dChoice - 1], sizeof(booking.doctorName) - 1);
    booking.doctorName[sizeof(booking.doctorName) - 1] = '\0';

    inputString(booking.bookingDate, sizeof(booking.bookingDate),
                "Enter Booking Date (DD/MM/YYYY): ");

    strncpy(booking.userGmail, gmail, sizeof(booking.userGmail) - 1);
    booking.userGmail[sizeof(booking.userGmail) - 1] = '\0';

    strncpy(booking.userNID, user.nid, sizeof(booking.userNID) - 1);
    booking.userNID[sizeof(booking.userNID) - 1] = '\0';

    file = fopen(BOOKING_FILE, "a");
    if (!file) { printf("\nError: Cannot save booking!\n"); return; }

    fprintf(file, "%s|%s|%s|%s|%s\n",
            booking.userGmail, booking.userNID,
            booking.hospitalName, booking.doctorName,
            booking.bookingDate);

    fclose(file);

    printf("\n+======================================================+\n");
    printf("|           BOOKING CONFIRMED SUCCESSFULLY!            |\n");
    printf("+======================================================+\n");
    printf("  Patient  : %s\n", user.name);
    printf("  NID      : %s\n", user.nid);
    printf("  Hospital : %s\n", booking.hospitalName);
    printf("  Doctor   : %s\n", booking.doctorName);
    printf("  Date     : %s\n", booking.bookingDate);
    printf("+------------------------------------------------------+\n");
}


/* ================================================
   VIEW MY BOOKINGS
   ================================================ */

void viewMyBookings(void)
{
    char gmail[100], password[100];
    struct User user;
    struct Booking booking;
    FILE *file;
    int count = 0;

    printf("\n+======================================================+\n");
    printf("|                  MY BOOKINGS                         |\n");
    printf("+======================================================+\n");

    inputString(gmail,    sizeof(gmail),    "Enter your Gmail    : ");
    inputString(password, sizeof(password), "Enter your Password : ");

    if (!findUser(gmail, password, &user))
    {
        printf("\nInvalid Gmail or Password! Access Denied.\n");
        return;
    }

    file = fopen(BOOKING_FILE, "r");
    if (!file) { printf("\nNo bookings found.\n"); return; }

    printf("\nBookings for: %s  (NID: %s)\n", user.name, user.nid);
    printf("+------------------------------------------------------+\n");

    while (fscanf(file,
                  "%99[^|]|%19[^|]|%149[^|]|%149[^|]|%29[^\n]\n",
                  booking.userGmail, booking.userNID,
                  booking.hospitalName, booking.doctorName,
                  booking.bookingDate) == 5)
    {
        if (strcmp(booking.userGmail, gmail) == 0)
        {
            count++;
            printf("  Booking #%d\n", count);
            printf("  NID      : %s\n", booking.userNID);
            printf("  Hospital : %s\n", booking.hospitalName);
            printf("  Doctor   : %s\n", booking.doctorName);
            printf("  Date     : %s\n", booking.bookingDate);
            printf("+------------------------------------------------------+\n");
        }
    }

    fclose(file);

    if (count == 0)
    {
        printf("  You have no bookings yet.\n");
        printf("+------------------------------------------------------+\n");
    }
}


/* ================================================
   CANCEL A BOOKING
   ================================================ */

void cancelBooking(void)
{
    char gmail[100], password[100], confirm[8];
    struct User user;
    struct Booking booking;
    FILE *file, *temp;
    int count = 0, cancelNo, current = 0, found = 0;

    printf("\n+======================================================+\n");
    printf("|                 CANCEL A BOOKING                     |\n");
    printf("+======================================================+\n");

    inputString(gmail,    sizeof(gmail),    "Enter your Gmail    : ");
    inputString(password, sizeof(password), "Enter your Password : ");

    if (!findUser(gmail, password, &user))
    {
        printf("\nInvalid Gmail or Password! Access Denied.\n");
        return;
    }

    file = fopen(BOOKING_FILE, "r");
    if (!file) { printf("\nNo bookings found.\n"); return; }

    printf("\nYour current bookings:\n");
    printf("+------------------------------------------------------+\n");

    while (fscanf(file,
                  "%99[^|]|%19[^|]|%149[^|]|%149[^|]|%29[^\n]\n",
                  booking.userGmail, booking.userNID,
                  booking.hospitalName, booking.doctorName,
                  booking.bookingDate) == 5)
    {
        if (strcmp(booking.userGmail, gmail) == 0)
        {
            count++;
            printf("  Booking #%d\n", count);
            printf("  Hospital : %s\n", booking.hospitalName);
            printf("  Doctor   : %s\n", booking.doctorName);
            printf("  Date     : %s\n", booking.bookingDate);
            printf("+------------------------------------------------------+\n");
        }
    }
    fclose(file);

    if (count == 0)
    {
        printf("  You have no bookings to cancel.\n");
        return;
    }

    cancelNo = readInt("Enter Booking Number to cancel: ", 1, count);

    inputString(confirm, sizeof(confirm),
                "Are you sure? Enter Y to confirm / N to cancel: ");

    if (confirm[0] != 'Y' && confirm[0] != 'y')
    {
        printf("\nCancellation aborted.\n");
        return;
    }

    file = fopen(BOOKING_FILE, "r");
    temp = fopen(TEMP_BOOKING, "w");

    if (!file || !temp)
    {
        printf("\nFile error!\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    while (fscanf(file,
                  "%99[^|]|%19[^|]|%149[^|]|%149[^|]|%29[^\n]\n",
                  booking.userGmail, booking.userNID,
                  booking.hospitalName, booking.doctorName,
                  booking.bookingDate) == 5)
    {
        if (strcmp(booking.userGmail, gmail) == 0)
        {
            current++;
            if (current == cancelNo) { found = 1; continue; }
        }

        fprintf(temp, "%s|%s|%s|%s|%s\n",
                booking.userGmail, booking.userNID,
                booking.hospitalName, booking.doctorName,
                booking.bookingDate);
    }

    fclose(file);
    fclose(temp);

    if (found)
    {
        remove(BOOKING_FILE);
        rename(TEMP_BOOKING, BOOKING_FILE);
        printf("\n+======================================================+\n");
        printf("|          BOOKING CANCELLED SUCCESSFULLY!             |\n");
        printf("+======================================================+\n");
    }
    else
    {
        remove(TEMP_BOOKING);
        printf("\nSomething went wrong. Booking not cancelled.\n");
    }
}


/* ================================================
   ADMIN — VIEW ALL USERS
   ================================================ */

void viewAllUsers(void)
{
    struct User user;
    FILE *file;
    int count = 0;

    file = fopen(USER_FILE, "r");
    if (!file) { printf("\nNo users registered yet.\n"); return; }

    printf("\n+======================================================+\n");
    printf("|              REGISTERED USER LIST                    |\n");
    printf("+======================================================+\n");

    while (fscanf(file,
                  "%99[^|]|%99[^|]|%99[^|]|%149[^|]|%19[^|]|%19[^|]|%d|%19[^\n]\n",
                  user.name, user.gmail, user.password,
                  user.address, user.phone, user.nid,
                  &user.age, user.gender) == 8)
    {
        count++;
        printf("\n  User #%d\n", count);
        printf("  Name    : %s\n", user.name);
        printf("  Gmail   : %s\n", user.gmail);
        printf("  Address : %s\n", user.address);
        printf("  Phone   : %s\n", user.phone);
        printf("  NID     : %s\n", user.nid);
        printf("  Age     : %d\n", user.age);
        printf("  Gender  : %s\n", user.gender);
        printf("+------------------------------------------------------+\n");
    }

    fclose(file);

    if (count == 0)
        printf("  No users found.\n");
    else
        printf("\n  Total Registered Users: %d\n", count);
}


/* ================================================
   ADMIN — VIEW ALL BOOKINGS
   ================================================ */

void viewAllBookings(void)
{
    struct Booking booking;
    FILE *file;
    int count = 0;

    file = fopen(BOOKING_FILE, "r");
    if (!file) { printf("\nNo bookings found.\n"); return; }

    printf("\n+======================================================+\n");
    printf("|                  ALL BOOKINGS                        |\n");
    printf("+======================================================+\n");

    while (fscanf(file,
                  "%99[^|]|%19[^|]|%149[^|]|%149[^|]|%29[^\n]\n",
                  booking.userGmail, booking.userNID,
                  booking.hospitalName, booking.doctorName,
                  booking.bookingDate) == 5)
    {
        count++;
        printf("\n  Booking #%d\n", count);
        printf("  Patient Gmail : %s\n", booking.userGmail);
        printf("  Patient NID   : %s\n", booking.userNID);
        printf("  Hospital      : %s\n", booking.hospitalName);
        printf("  Doctor        : %s\n", booking.doctorName);
        printf("  Date          : %s\n", booking.bookingDate);
        printf("+------------------------------------------------------+\n");
    }

    fclose(file);

    if (count == 0)
        printf("  No bookings found.\n");
    else
        printf("\n  Total Bookings: %d\n", count);
}


/* ================================================
   PATIENT PANEL
   ================================================ */

void patientPanel(void)
{
    int choice;

    while (1)
    {
        printf("\n+======================================================+\n");
        printf("|                   PATIENT PANEL                      |\n");
        printf("+======================================================+\n");
        printf("|  1. Register New Account                             |\n");
        printf("|  2. View My Information                              |\n");
        printf("|  3. Update My Information                            |\n");
        printf("|  4. Delete My Account                                |\n");
        printf("|  5. Book a Doctor                                    |\n");
        printf("|  6. View My Bookings                                 |\n");
        printf("|  7. Cancel a Booking                                 |\n");
        printf("|  8. Back                                             |\n");
        printf("+------------------------------------------------------+\n");

        choice = readInt("=> Enter choice: ", 1, 8);

        switch (choice)
        {
            case 1: registerUser();        break;
            case 2: viewMyInformation();   break;
            case 3: updateMyInformation(); break;
            case 4: deleteMyInformation(); break;
            case 5: bookDoctor();          break;
            case 6: viewMyBookings();      break;
            case 7: cancelBooking();       break;
            case 8: return;
        }
    }
}

