// Restaurant Billing & Payment - IU2541230457 - CPP27
// main() only creates the system object, shows the menu and calls system functions.
#include <iostream>
#include <exception>
#include "RestaurantSystem.h"
#include "Utils.h"

static void printMainMenu()
{
    std::cout << "\n========================================\n"
              << "   RESTAURANT BILLING & PAYMENT SYSTEM\n"
              << "========================================\n"
              << " 1. Add Record\n"
              << " 2. Display Records\n"
              << " 3. Search Record\n"
              << " 4. Update Record\n"
              << " 5. Delete Record\n"
              << " 6. Main Transaction (Order -> Bill -> Payment)\n"
              << " 7. Report\n"
              << " 8. Save / Load\n"
              << " 9. Exit\n"
              << "========================================\n";
}

int main()
{
    RestaurantSystem system;   // loads the saved records in its constructor

    try {
        bool running = true;
        while (running) {
            try {
                printMainMenu();
                int choice = readInt("Enter your choice (1-9): ", 1, 9);
                switch (choice) {
                    case 1: system.addRecord();       break;
                    case 2: system.displayRecords();  break;
                    case 3: system.searchRecord();    break;
                    case 4: system.updateRecord();    break;
                    case 5: system.deleteRecord();    break;
                    case 6: system.mainTransaction(); break;
                    case 7: system.report();          break;
                    case 8: system.saveLoadMenu();    break;
                    case 9: running = !system.exitProgram(); break;
                }
            } catch (const InputClosedError&) {
                throw;   // handled by the outer try
            } catch (const std::exception& e) {
                // Any unexpected error: report it and return to the menu.
                std::cout << "\nUnexpected error: " << e.what() << "\n";
            }
        }
    } catch (const InputClosedError&) {
        std::cout << "\nKeyboard input ended. Saving and closing.\n";
        system.autoSave();
    }
    return 0;
}
