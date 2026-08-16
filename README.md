# District Management System

A comprehensive C-based console application for managing essential district services including hospitals, restaurants, and shopping malls. This system provides separate management panels for service providers and customers/patients, with robust authentication and data management features.

---

## 📋 Table of Contents

- [Features](#features)
- [System Architecture](#system-architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Module Details](#module-details)
- [Data Storage](#data-storage)
- [Contributors](#contributors)
- [License](#license)

---

## ✨ Features

### 🏥 Hospital Management (by Sinthia)
- **Patient Panel**
  - User registration with email and password validation
  - Secure login with credential verification
  - Profile management (view, update, delete)
  - Doctor appointment booking across 4 major hospitals
  - Booking history tracking
  - Appointment cancellation

- **Admin Panel**
  - View all registered users
  - View all booking records
  - Password-protected administrative access

- **Hospitals Covered**
  - Dhaka Medical College Hospital
  - Sir Salimullah Medical College Hospital
  - Shaheed Suhrawardy Medical College Hospital
  - National Institute of Cancer Research Hospital

### 🍽️ Restaurant Management (by Sumaiya)
- **Restaurant Owner Panel**
  - Add new restaurant with validated credentials
  - View all restaurants with ratings and feedback
  - Update restaurant information (including owner Gmail)
  - Delete restaurant with confirmation
  - Duplicate Gmail prevention

- **Customer Panel**
  - Customer registration and authentication
  - Browse all restaurants
  - Multi-category rating system (Waiter Behavior, Food Quality, Environment)
  - Submit text feedback for restaurants
  - View aggregated ratings and reviews

### 🛍️ Shopping Mall Management (by Halima)
- **Mall Owner Panel**
  - Add shopping mall with validated credentials
  - View all malls with ratings and feedback
  - Update mall information (including owner Gmail)
  - Delete mall with confirmation
  - Duplicate Gmail prevention

- **Buyer Panel**
  - Buyer registration and authentication
  - Browse all shopping malls
  - Multi-category rating system (Shop Variety, Cleanliness, Security)
  - Submit text feedback for malls
  - View aggregated ratings and reviews

---

## 🏗️ System Architecture

### Project Structure
```
district-management-system/
├── main.c                    # Main menu and program entry point
├── hospital.c                # Hospital management module
├── restaurant.c              # Restaurant management module
├── shopping_mall.c           # Shopping mall management module
├── users.txt                 # Global user database (8-field format)
├── bookings.txt              # Hospital appointment records
├── restaurants.txt           # Restaurant data with ratings
├── restaurant_feedbacks.txt  # Customer feedback for restaurants
├── malls.txt                 # Shopping mall data with ratings
├── mall_feedbacks.txt        # Customer feedback for malls
├── MakeFile                  # Build automation
└── README.md                 # Project documentation
```

### Data Format Specifications

**users.txt** (8 fields):
```
name|gmail|password|address|phone|nid|age|gender
```

**restaurants.txt / malls.txt** (11 fields):
```
name|location|owner|gmail|password|rating1|count1|rating2|count2|rating3|count3
```

**bookings.txt** (5 fields):
```
userGmail|userNID|hospitalName|doctorName|bookingDate
```

**feedback files** (3 fields):
```
entityName|userGmail|feedbackMessage
```

---

## 🚀 Installation

### Prerequisites
- GCC compiler (version 4.8 or higher)
- Linux/Unix environment or Windows with MinGW
- Make utility (optional, for automated builds)

### Build Instructions

**Using Make:**
```bash
make
```

**Manual Compilation:**
```bash
gcc -Wall -Wextra -Wshadow -o district_management main.c hospital.c restaurant.c shopping_mall.c
```

**Clean Build:**
```bash
make clean
```

---

## 💻 Usage

### Running the Application
```bash
./district_management
```

### Main Menu Options
1. **Hospital Management** - Access hospital services
2. **Restaurant Management** - Manage restaurants and dining
3. **Shopping Mall Management** - Manage shopping centers
4. **Exit** - Close the application

### User Workflows

#### For Patients/Customers
1. Navigate to desired service (Hospital/Restaurant/Mall)
2. Register a new account with valid email (must contain `@` and `.com`)
3. Login with credentials
4. Access services (book appointments, rate establishments, submit feedback)

#### For Service Providers
1. Navigate to owner/admin panel
2. Login with existing credentials or create new establishment
3. Manage your establishment (add, update, delete)
4. View ratings and feedback

---

## 📦 Module Details

### Hospital Module (hospital.c)
**Key Features:**
- NID-based patient tracking
- Multi-hospital, multi-doctor booking system
- Email validation with retry mechanism
- Password strength enforcement (minimum 6 characters)
- Admin panel with password protection (default: `admin1234`)

**Validation Rules:**
- Email must contain `@` and `.com`
- Password minimum length: 6 characters
- Age range: 1-120 years
- All fields mandatory

### Restaurant Module (restaurant.c)
**Key Features:**
- 3-tier rating system (Waiter, Food, Environment)
- Real-time rating calculation with weighted averages
- Text-based feedback system
- Case-insensitive restaurant name search
- Owner Gmail update capability

**Rating Categories:**
- Waiter Behavior (1-5 stars)
- Food Quality (1-5 stars)
- Environment (1-5 stars)

### Shopping Mall Module (shopping_mall.c)
**Key Features:**
- 3-tier rating system (Variety, Cleanliness, Security)
- Real-time rating calculation with weighted averages
- Text-based feedback system
- Case-insensitive mall name search
- Owner Gmail update capability

**Rating Categories:**
- Shop Variety (1-5 stars)
- Cleanliness (1-5 stars)
- Security (1-5 stars)

---

## 🗄️ Data Storage

### File Management
- All data stored in plain text files with pipe-delimited format
- Atomic file operations using temporary files for updates/deletes
- Data persistence across application restarts
- No external database dependencies

### Security Features
- Password validation (minimum length enforcement)
- Email format validation
- Duplicate registration prevention
- Confirmation prompts for destructive operations
- Admin password protection for sensitive operations

---

## 👥 Contributors

| Name | Module | Contribution |
|------|--------|--------------|
| **Halima** | Shopping Mall Management | Complete implementation of mall owner panel, buyer panel, rating system, and feedback mechanism |
| **Sinthia** | Hospital Management | Complete implementation of patient panel, admin panel, booking system, and hospital data structure |
| **Sumaiya** | Restaurant Management | Complete implementation of restaurant owner panel, customer panel, rating system, and feedback mechanism |

---

## 🔧 Technical Specifications

### Development Environment
- **Language:** C (C89/C90 compatible)
- **Compiler:** GCC with strict warning flags (`-Wall -Wextra -Wshadow`)
- **Platform:** Cross-platform (Linux, macOS, Windows)
- **Build System:** Make

### Code Quality
- Zero compiler warnings
- Zero compiler errors
- Input validation on all user inputs
- Memory-safe string operations using `strncpy` and bounds checking
- Proper file handle management (all opened files are closed)

### Input Handling
- Safe integer input with `fgets` + `sscanf` (no buffer overflow)
- String input with automatic newline removal
- Range validation for numeric inputs
- Retry loops for invalid inputs

---

## 📝 Known Limitations

1. **Data Storage:** Plain text files (no encryption for passwords)
2. **Concurrency:** Single-user application (no multi-user support)
3. **Search:** Basic string matching (no fuzzy search or advanced filtering)
4. **Scalability:** Linear search operations (not optimized for large datasets)

---

## 🔮 Future Enhancements

- [ ] Password encryption (hashing with salt)
- [ ] Advanced search and filtering options
- [ ] Export reports to CSV/PDF
- [ ] Multi-language support
- [ ] Database integration (SQLite/MySQL)
- [ ] Web-based interface
- [ ] User role-based access control
- [ ] Email notification system

---

## 📄 License

This project is developed as part of academic coursework.

**Course:** CSE 1290  
**Institution:** Northern University of Bangladesh (NUB)  
**Academic Year:** 2024-2025

---

## 📞 Support

For bug reports, feature requests, or questions, please contact the development team through your course instructor.

---

## 🎓 Acknowledgments

- Course Instructor for guidance and requirements specification
- All team members for their dedicated contributions
- Northern University of Bangladesh for providing the learning platform

---

**Last Updated:** August 15, 2026  
**Version:** 1.0.0  
**Status:** Production Ready ✅
