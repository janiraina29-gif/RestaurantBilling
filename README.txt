# Restaurant Billing & Payment System

**Project Code:** CPP27  
**Enrollment Number:** IU2541230457

## 1. Project Overview

The Restaurant Billing & Payment System is a menu-driven C++ application designed to manage restaurant menu items, customers, orders, bills, and payments. It provides a terminal-based interface for restaurant staff to perform common billing and management operations.

The application stores its records in text files so that data can be loaded when the program starts and saved for future use.

## 2. Features

- Add, display, search, update, and delete menu records.
- Manage customer records.
- Create restaurant orders using menu items and quantities.
- Generate bills with discounts, tax, and total calculations.
- Support cash, card, and UPI payment methods, with an option to pay later.
- Display reports for restaurant records and transactions.
- Save and load records using text files.
- Validate user input and handle errors.

## 3. Object-Oriented Programming Concepts

- **Classes and Objects:** Represent menu items, customers, orders, bills, payments, and the restaurant system.
- **Encapsulation:** Organizes data and related operations within classes.
- **Constructors and Destructors:** Support object initialization and resource management.
- **Inheritance:** Cash, card, and UPI payment classes derive from the abstract Payment class.
- **Runtime Polymorphism:** Payment operations use virtual functions through the base Payment interface.
- **Dynamic Memory Management:** Dynamic arrays and payment objects are managed in the program.
- **File Handling:** Records are read from and written to text files.
- **Exception Handling:** Validation errors are handled using exceptions.

## 4. Technologies Used

- Programming Language: C++
- Compiler: G++ (MinGW on Windows)
- Development Environment: Visual Studio Code
- Data Storage: Text files

## 5. Project Structure

- `Source/` — C++ source and header files.
- `Data/` — Text files used to store application records.
- `Executable/` — Compiled application executable.
- `Diagrams/` — UML diagrams.
- `Screenshots/` — Screenshots demonstrating program execution.
- `build.bat` — Windows batch file used to compile the project.

## 6. Build and Run Instructions

### Compile the project

Open PowerShell or the VS Code terminal in the project directory and run:

```powershell
.\build.bat
```

### Run the application

```powershell
.\Executable\Project.exe
```

Use the displayed menu to navigate through the application's operations.

## 7. Data Persistence

The application uses text files in the `Data/` directory to store menu items, customers, orders, order items, bills, payments, and supporting metadata. Saved records can be loaded when the application restarts.

## 8. UML Diagrams and Screenshots

The `Diagrams/` directory contains the use case diagram, class diagram, main transaction sequence diagram, and menu item update sequence diagram.

The `Screenshots/` directory contains screenshots demonstrating the application's interface and major operations.

## 9. Conclusion

The Restaurant Billing & Payment System demonstrates the application of C++ object-oriented programming, dynamic memory management, file handling, input validation, and payment polymorphism in a practical terminal-based restaurant management application.