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
