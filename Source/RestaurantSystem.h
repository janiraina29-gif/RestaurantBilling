#ifndef RESTAURANTSYSTEM_H
#define RESTAURANTSYSTEM_H

#include <string>
#include "DynamicArray.h"
#include "MenuItem.h"
#include "Customer.h"
#include "Order.h"
#include "Bill.h"
#include "Payment.h"
#include "FileManager.h"

// The "system class" of the Blueprint: owns all the data and performs every
// operation. main() only shows the menu and calls these public functions.
class RestaurantSystem {
private:
    DynamicArray<MenuItem> menu;
    DynamicArray<Customer> customers;
    DynamicArray<Order> orders;
    DynamicArray<Bill> bills;
    DynamicArray<Payment*> payments;   // OWNING pointers: every one is deleted by clearPayments()

    FileManager files;
    int nextMenuId;
    int nextCustomerId;
    int nextOrderId;
    int nextBillId;
    int nextPaymentId;
    bool dirty;        // true when there are changes not yet saved
    bool loadFailed;   // true if the data files could not be loaded at start

    // The system owns raw pointers, so copying it would be dangerous: forbid it.
    RestaurantSystem(const RestaurantSystem&) = delete;
    RestaurantSystem& operator=(const RestaurantSystem&) = delete;

    // ----- lookups (return an index, or -1 if not found) -----
    int findMenu(int id) const;
    int findCustomer(int id) const;
    int findOrder(int id) const;
    int findBill(int id) const;
    int findBillByOrder(int orderId) const;
    bool menuNameExists(const std::string& name, int exceptId) const;
    bool phoneExists(const std::string& phone, int exceptId) const;
    bool menuItemIsUsed(int menuItemId) const;
    bool customerHasOrders(int customerId) const;
    std::string customerName(int customerId) const;

    // ----- display helpers -----
    void showMenuItems() const;
    void showCustomers() const;
    void showOrders() const;
    void showBills() const;
    void showPayments() const;

    // ----- Add / Search / Update / Delete for each entity -----
    void addMenuItem();
    void addCustomer();
    void searchMenuItems() const;
    void searchCustomers() const;
    void searchOrders() const;
    void searchBills() const;
    void updateMenuItem();
    void updateCustomer();
    void deleteMenuItem();
    void deleteCustomer();
    void cancelOrder();

    // ----- main transaction -----
    void placeOrder();
    void payExistingBill();
    bool collectPayment(int billIndex);   // true if the bill got paid

    // ----- persistence -----
    void clearPayments();
    void clearAll();
    void loadAll();                       // clear, then load; reports problems
    void loadFromFiles();                 // throws std::runtime_error on bad data
    Payment* paymentFromLine(const std::string& line) const;
    void saveFiles() const;               // throws std::runtime_error on failure
    bool saveWithMessage();

public:
    RestaurantSystem();    // loads existing records
    ~RestaurantSystem();   // releases all dynamic memory

    // The nine menu operations of the Blueprint:
    void addRecord();
    void displayRecords() const;
    void searchRecord() const;
    void updateRecord();
    void deleteRecord();
    void mainTransaction();
    void report() const;
    void saveLoadMenu();
    bool exitProgram();    // saves; returns true if the program may quit

    // Used by main() if the keyboard input disappears unexpectedly.
    void autoSave();
};

#endif
