#include "RestaurantSystem.h"
#include "PaymentTypes.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

// ===========================================================================
// Construction / destruction
// ===========================================================================

RestaurantSystem::RestaurantSystem()
    : menu(), customers(), orders(), bills(), payments(), files(),
      nextMenuId(1), nextCustomerId(1), nextOrderId(1), nextBillId(1), nextPaymentId(1),
      dirty(false), loadFailed(false)
{
    loadAll();
}

RestaurantSystem::~RestaurantSystem()
{
    clearPayments();   // delete every Payment object we created with 'new'
    // The DynamicArray members free their own arrays in their destructors.
}

// ===========================================================================
// Lookups
// ===========================================================================

int RestaurantSystem::findMenu(int id) const
{
    for (int i = 0; i < menu.size(); i++) {
        if (menu[i].getId() == id) return i;
    }
    return -1;
}

int RestaurantSystem::findCustomer(int id) const
{
    for (int i = 0; i < customers.size(); i++) {
        if (customers[i].getId() == id) return i;
    }
    return -1;
}

int RestaurantSystem::findOrder(int id) const
{
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getId() == id) return i;
    }
    return -1;
}

int RestaurantSystem::findBill(int id) const
{
    for (int i = 0; i < bills.size(); i++) {
        if (bills[i].getId() == id) return i;
    }
    return -1;
}

int RestaurantSystem::findBillByOrder(int orderId) const
{
    for (int i = 0; i < bills.size(); i++) {
        if (bills[i].getOrderId() == orderId) return i;
    }
    return -1;
}

bool RestaurantSystem::menuNameExists(const std::string& name, int exceptId) const
{
    for (int i = 0; i < menu.size(); i++) {
        if (menu[i].getId() != exceptId && toLower(menu[i].getName()) == toLower(trim(name))) {
            return true;
        }
    }
    return false;
}

bool RestaurantSystem::phoneExists(const std::string& phone, int exceptId) const
{
    for (int i = 0; i < customers.size(); i++) {
        if (customers[i].getId() != exceptId && customers[i].getPhone() == trim(phone)) {
            return true;
        }
    }
    return false;
}

bool RestaurantSystem::menuItemIsUsed(int menuItemId) const
{
    for (int i = 0; i < orders.size(); i++) {
        for (int j = 0; j < orders[i].getLineCount(); j++) {
            if (orders[i].getLine(j).menuItemId == menuItemId) return true;
        }
    }
    return false;
}

bool RestaurantSystem::customerHasOrders(int customerId) const
{
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getCustomerId() == customerId) return true;
    }
    return false;
}

std::string RestaurantSystem::customerName(int customerId) const
{
    int idx = findCustomer(customerId);
    return (idx >= 0) ? customers[idx].getName() : std::string("(unknown)");
}

// ===========================================================================
// Display helpers
// ===========================================================================

void RestaurantSystem::showMenuItems() const
{
    if (menu.size() == 0) {
        std::cout << "(no menu items yet)\n";
        return;
    }
    MenuItem::printHeader();
    for (int i = 0; i < menu.size(); i++) menu[i].display();
}

void RestaurantSystem::showCustomers() const
{
    if (customers.size() == 0) {
        std::cout << "(no customers yet)\n";
        return;
    }
    Customer::printHeader();
    for (int i = 0; i < customers.size(); i++) customers[i].display();
}

void RestaurantSystem::showOrders() const
{
    if (orders.size() == 0) {
        std::cout << "(no orders yet)\n";
        return;
    }
    for (int i = 0; i < orders.size(); i++) {
        orders[i].display(customerName(orders[i].getCustomerId()));
        std::cout << std::endl;
    }
}

void RestaurantSystem::showBills() const
{
    if (bills.size() == 0) {
        std::cout << "(no bills yet)\n";
        return;
    }
    Bill::printHeader();
    for (int i = 0; i < bills.size(); i++) bills[i].displayRow();
}

void RestaurantSystem::showPayments() const
{
    if (payments.size() == 0) {
        std::cout << "(no payments yet)\n";
        return;
    }
    Payment::printHeader();
    for (int i = 0; i < payments.size(); i++) payments[i]->displayRow();
}

// ===========================================================================
// 1. Add Record
// ===========================================================================

void RestaurantSystem::addRecord()
{
    std::cout << "\n--- Add Record ---\n"
              << "1. Menu item\n"
              << "2. Customer\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 2);
    if (choice == 1) addMenuItem();
    else if (choice == 2) addCustomer();
}

void RestaurantSystem::addMenuItem()
{
    std::cout << "\n--- Add Menu Item ---\n";
    std::string name = readText("Name: ");
    std::string category = readText("Category (e.g. Starter, Main, Drinks, Dessert): ");
    Money price = readMoney("Price in rupees (e.g. 120 or 120.50): ");

    if (menuNameExists(name, 0)) {
        std::cout << "A menu item with that name already exists.\n";
        return;
    }
    try {
        MenuItem item(nextMenuId, name, category, price);
        menu.add(item);
        nextMenuId++;
        dirty = true;
        std::cout << "Added menu item #" << item.getId() << ".\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Could not add item: " << e.what() << "\n";
    }
}

void RestaurantSystem::addCustomer()
{
    std::cout << "\n--- Add Customer ---\n";
    std::string name = readText("Name: ");
    std::string phone = readText("Phone (10 digits): ");

    if (phoneExists(phone, 0)) {
        std::cout << "A customer with that phone number already exists.\n";
        return;
    }
    try {
        Customer c(nextCustomerId, name, phone);
        customers.add(c);
        nextCustomerId++;
        dirty = true;
        std::cout << "Added customer #" << c.getId() << ".\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Could not add customer: " << e.what() << "\n";
    }
}

// ===========================================================================
// 2. Display Records
// ===========================================================================

void RestaurantSystem::displayRecords() const
{
    std::cout << "\n--- Display Records ---\n"
              << "1. Menu items\n"
              << "2. Customers\n"
              << "3. Orders\n"
              << "4. Bills\n"
              << "5. Payments\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 5);
    std::cout << std::endl;
    switch (choice) {
        case 1: showMenuItems(); break;
        case 2: showCustomers(); break;
        case 3: showOrders(); break;
        case 4: showBills(); break;
        case 5: showPayments(); break;
        default: break;
    }
}

// ===========================================================================
// 3. Search Record
// ===========================================================================

void RestaurantSystem::searchRecord() const
{
    std::cout << "\n--- Search Record ---\n"
              << "1. Menu item (by name, category or ID)\n"
              << "2. Customer (by name, phone or ID)\n"
              << "3. Order (by order ID or customer ID)\n"
              << "4. Bill (by bill ID)\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 4);
    switch (choice) {
        case 1: searchMenuItems(); break;
        case 2: searchCustomers(); break;
        case 3: searchOrders(); break;
        case 4: searchBills(); break;
        default: break;
    }
}

void RestaurantSystem::searchMenuItems() const
{
    std::string key = readText("Search text: ");
    int found = 0;
    for (int i = 0; i < menu.size(); i++) {
        const MenuItem& m = menu[i];
        if (containsIgnoreCase(m.getName(), key) || containsIgnoreCase(m.getCategory(), key) ||
            std::to_string(m.getId()) == key) {
            if (found == 0) MenuItem::printHeader();
            m.display();
            found++;
        }
    }
    std::cout << found << " match(es) found.\n";
}

void RestaurantSystem::searchCustomers() const
{
    std::string key = readText("Search text: ");
    int found = 0;
    for (int i = 0; i < customers.size(); i++) {
        const Customer& c = customers[i];
        if (containsIgnoreCase(c.getName(), key) || c.getPhone().find(key) != std::string::npos ||
            std::to_string(c.getId()) == key) {
            if (found == 0) Customer::printHeader();
            c.display();
            found++;
        }
    }
    std::cout << found << " match(es) found.\n";
}

void RestaurantSystem::searchOrders() const
{
    std::cout << "1. By order ID\n2. By customer ID\n";
    int how = readInt("Choose: ", 1, 2);
    int id = readInt("Enter the ID: ", 1, MAX_INPUT_NUMBER);
    int found = 0;
    for (int i = 0; i < orders.size(); i++) {
        bool match = (how == 1) ? (orders[i].getId() == id) : (orders[i].getCustomerId() == id);
        if (match) {
            orders[i].display(customerName(orders[i].getCustomerId()));
            std::cout << std::endl;
            found++;
        }
    }
    std::cout << found << " match(es) found.\n";
}

void RestaurantSystem::searchBills() const
{
    int id = readInt("Bill ID: ", 1, MAX_INPUT_NUMBER);
    int idx = findBill(id);
    if (idx < 0) {
        std::cout << "No bill with that ID.\n";
        return;
    }
    bills[idx].display();
    int pay = -1;
    for (int i = 0; i < payments.size(); i++) {
        if (payments[i]->getBillId() == id) pay = i;
    }
    if (pay >= 0) {
        std::cout << "Paid by:\n";
        payments[pay]->display();
    }
}

// ===========================================================================
// 4. Update Record
// ===========================================================================

void RestaurantSystem::updateRecord()
{
    std::cout << "\n--- Update Record ---\n"
              << "1. Menu item\n"
              << "2. Customer\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 2);
    if (choice == 1) updateMenuItem();
    else if (choice == 2) updateCustomer();
}

void RestaurantSystem::updateMenuItem()
{
    showMenuItems();
    int id = readInt("Menu item ID to update (0 = cancel): ", 0, MAX_INPUT_NUMBER);
    if (id == 0) return;
    int idx = findMenu(id);
    if (idx < 0) {
        std::cout << "No menu item with that ID.\n";
        return;
    }

    // Work on a COPY; only overwrite the real record if every change is valid.
    MenuItem edited = menu[idx];
    std::cout << "Press Enter on any question to keep the current value.\n";
    try {
        std::string s = readOptional("Name [" + edited.getName() + "]: ");
        if (!s.empty()) {
            if (menuNameExists(s, id)) {
                std::cout << "Another menu item already has that name. Update cancelled.\n";
                return;
            }
            edited.setName(s);
        }
        s = readOptional("Category [" + edited.getCategory() + "]: ");
        if (!s.empty()) edited.setCategory(s);

        s = readOptional("Price [" + formatMoney(edited.getPrice()) + "]: ");
        if (!s.empty()) {
            Money price = 0;
            if (!parseMoney(s, price)) {
                std::cout << "Invalid price. Update cancelled.\n";
                return;
            }
            edited.setPrice(price);
        }

        s = toLower(readOptional(std::string("Available (y/n) [") + (edited.isAvailable() ? "y" : "n") + "]: "));
        if (s == "y" || s == "yes") edited.setAvailable(true);
        else if (s == "n" || s == "no") edited.setAvailable(false);
        else if (!s.empty()) {
            std::cout << "Invalid answer. Update cancelled.\n";
            return;
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Update cancelled: " << e.what() << "\n";
        return;
    }
    menu[idx] = edited;
    dirty = true;
    std::cout << "Menu item updated. (Old orders and bills keep their original price.)\n";
}

void RestaurantSystem::updateCustomer()
{
    showCustomers();
    int id = readInt("Customer ID to update (0 = cancel): ", 0, MAX_INPUT_NUMBER);
    if (id == 0) return;
    int idx = findCustomer(id);
    if (idx < 0) {
        std::cout << "No customer with that ID.\n";
        return;
    }

    Customer edited = customers[idx];
    std::cout << "Press Enter on any question to keep the current value.\n";
    try {
        std::string s = readOptional("Name [" + edited.getName() + "]: ");
        if (!s.empty()) edited.setName(s);
        s = readOptional("Phone [" + edited.getPhone() + "]: ");
        if (!s.empty()) {
            if (phoneExists(s, id)) {
                std::cout << "Another customer already has that phone number. Update cancelled.\n";
                return;
            }
            edited.setPhone(s);
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Update cancelled: " << e.what() << "\n";
        return;
    }
    customers[idx] = edited;
    dirty = true;
    std::cout << "Customer updated.\n";
}

// ===========================================================================
// 5. Delete Record
// ===========================================================================

void RestaurantSystem::deleteRecord()
{
    std::cout << "\n--- Delete Record ---\n"
              << "1. Menu item\n"
              << "2. Customer\n"
              << "3. Cancel an unpaid order\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 3);
    if (choice == 1) deleteMenuItem();
    else if (choice == 2) deleteCustomer();
    else if (choice == 3) cancelOrder();
}

void RestaurantSystem::deleteMenuItem()
{
    showMenuItems();
    int id = readInt("Menu item ID to delete (0 = cancel): ", 0, MAX_INPUT_NUMBER);
    if (id == 0) return;
    int idx = findMenu(id);
    if (idx < 0) {
        std::cout << "No menu item with that ID.\n";
        return;
    }
    if (menuItemIsUsed(id)) {
        // Erasing it would not break old orders (they keep their own copy of the
        // name and price) but we keep the record so history stays traceable.
        std::cout << "This item appears in past orders, so it cannot be erased.\n";
        if (readYesNo("Mark it as unavailable instead? (y/n): ")) {
            menu[idx].setAvailable(false);
            dirty = true;
            std::cout << "Item marked unavailable.\n";
        }
        return;
    }
    if (readYesNo("Delete '" + menu[idx].getName() + "' permanently? (y/n): ")) {
        menu.removeAt(idx);
        dirty = true;
        std::cout << "Menu item deleted.\n";
    }
}

void RestaurantSystem::deleteCustomer()
{
    showCustomers();
    int id = readInt("Customer ID to delete (0 = cancel): ", 0, MAX_INPUT_NUMBER);
    if (id == 0) return;
    int idx = findCustomer(id);
    if (idx < 0) {
        std::cout << "No customer with that ID.\n";
        return;
    }
    if (customerHasOrders(id)) {
        std::cout << "This customer has orders on record and cannot be deleted "
                     "(it would corrupt the transaction history).\n";
        return;
    }
    if (readYesNo("Delete '" + customers[idx].getName() + "' permanently? (y/n): ")) {
        customers.removeAt(idx);
        dirty = true;
        std::cout << "Customer deleted.\n";
    }
}

void RestaurantSystem::cancelOrder()
{
    bool any = false;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getStatus() == ORDER_PENDING) {
            orders[i].display(customerName(orders[i].getCustomerId()));
            std::cout << std::endl;
            any = true;
        }
    }
    if (!any) {
        std::cout << "There are no unpaid orders to cancel.\n";
        return;
    }
    int id = readInt("Order ID to cancel (0 = back): ", 0, MAX_INPUT_NUMBER);
    if (id == 0) return;
    int idx = findOrder(id);
    if (idx < 0) {
        std::cout << "No order with that ID.\n";
        return;
    }
    if (orders[idx].getStatus() != ORDER_PENDING) {
        std::cout << "Only unpaid (PENDING) orders can be cancelled.\n";
        return;
    }
    if (!readYesNo("Cancel order #" + std::to_string(id) + "? (y/n): ")) return;

    orders[idx].setStatus(ORDER_CANCELLED);
    int b = findBillByOrder(id);
    if (b >= 0) bills[b].markCancelled();
    dirty = true;
    std::cout << "Order and its bill are now CANCELLED (kept in the records).\n";
}

// ===========================================================================
// 6. Main transaction: order -> bill -> payment
// ===========================================================================

void RestaurantSystem::mainTransaction()
{
    std::cout << "\n--- Main Transaction ---\n"
              << "1. Place a new order (creates the bill, then takes payment)\n"
              << "2. Pay an existing unpaid bill\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 2);
    if (choice == 1) placeOrder();
    else if (choice == 2) payExistingBill();
}

void RestaurantSystem::placeOrder()
{
    std::cout << "\n--- Place New Order ---\n";
    if (customers.size() == 0) {
        std::cout << "Add a customer first (main menu option 1).\n";
        return;
    }
    bool anyAvailable = false;
    for (int i = 0; i < menu.size(); i++) {
        if (menu[i].isAvailable()) anyAvailable = true;
    }
    if (!anyAvailable) {
        std::cout << "There are no available menu items. Add one first (main menu option 1).\n";
        return;
    }

    showCustomers();
    int customerId = readInt("Customer ID (0 = cancel): ", 0, MAX_INPUT_NUMBER);
    if (customerId == 0) return;
    if (findCustomer(customerId) < 0) {
        std::cout << "No customer with that ID.\n";
        return;
    }

    try {
        Order order(nextOrderId, customerId);

        std::cout << "\nAvailable menu:\n";
        MenuItem::printHeader();
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].isAvailable()) menu[i].display();
        }

        while (true) {
            int itemId = readInt("\nMenu item ID to add (0 = finish): ", 0, MAX_INPUT_NUMBER);
            if (itemId == 0) break;
            int idx = findMenu(itemId);
            if (idx < 0 || !menu[idx].isAvailable()) {
                std::cout << "  That item is not available.\n";
                continue;
            }
            int quantity = readInt("  Quantity (1-99): ", 1, 99);
            try {
                order.addLine(menu[idx].getId(), menu[idx].getName(), menu[idx].getPrice(), quantity);
                std::cout << "  Added. Running subtotal: " << formatMoney(order.getSubtotal()) << "\n";
            } catch (const std::invalid_argument& e) {
                std::cout << "  " << e.what() << "\n";
            }
        }

        if (order.getLineCount() == 0) {
            std::cout << "No items chosen. No order was created.\n";
            return;
        }

        int discount = readInt("Discount percent (0-50): ", 0, 50);
        Bill bill(nextBillId, order.getId(), customerId, order.getSubtotal(), discount, currentDateTime());

        orders.add(order);
        bills.add(bill);
        nextOrderId++;
        nextBillId++;
        dirty = true;

        std::cout << "\nOrder created.\n";
        order.display(customerName(customerId));
        std::cout << std::endl;
        bill.display();

        if (readYesNo("Take payment now? (y/n): ")) {
            collectPayment(bills.size() - 1);
        } else {
            std::cout << "Bill left UNPAID. Use 'Pay an existing unpaid bill' later.\n";
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Could not create the order: " << e.what() << "\n";
    }
}

void RestaurantSystem::payExistingBill()
{
    bool any = false;
    for (int i = 0; i < bills.size(); i++) {
        if (bills[i].getStatus() == BILL_UNPAID) {
            if (!any) Bill::printHeader();
            bills[i].displayRow();
            any = true;
        }
    }
    if (!any) {
        std::cout << "There are no unpaid bills.\n";
        return;
    }
    int id = readInt("Bill ID to pay (0 = back): ", 0, MAX_INPUT_NUMBER);
    if (id == 0) return;
    int idx = findBill(id);
    if (idx < 0 || bills[idx].getStatus() != BILL_UNPAID) {
        std::cout << "That is not an unpaid bill.\n";
        return;
    }
    collectPayment(idx);
}

bool RestaurantSystem::collectPayment(int billIndex)
{
    Bill& bill = bills[billIndex];
    std::cout << "\nAmount due: " << formatMoney(bill.getTotal()) << "\n";

    while (true) {
        std::cout << "\nPayment method:  1. Cash   2. Card   3. UPI   0. Pay later\n";
        int method = readInt("Choose: ", 0, 3);
        if (method == 0) {
            std::cout << "Bill left UNPAID.\n";
            return false;
        }

        // 'new' builds a derived object, but we only keep a Payment* (base pointer).
        // If a constructor throws, 'new' frees the memory automatically.
        Payment* payment = nullptr;
        try {
            if (method == 1) {
                Money tendered = readMoney("Cash handed over (rupees): ");
                payment = new CashPayment(nextPaymentId, bill.getId(), bill.getTotal(), tendered);
            } else if (method == 2) {
                std::string last4 = readText("Last 4 digits of the card: ");
                payment = new CardPayment(nextPaymentId, bill.getId(), bill.getTotal(), last4);
            } else {
                std::string upi = readText("UPI ID (e.g. name@bank): ");
                payment = new UpiPayment(nextPaymentId, bill.getId(), bill.getTotal(), upi);
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "  Payment rejected: " << e.what() << "\n";
            continue;   // let the user try again or choose 0
        }

        try {
            payments.add(payment);
        } catch (...) {
            delete payment;   // never leak if the array could not store it
            throw;
        }

        nextPaymentId++;
        bill.markPaid();
        int o = findOrder(bill.getOrderId());
        if (o >= 0) orders[o].setStatus(ORDER_PAID);
        dirty = true;

        std::cout << "\nPayment successful.\n";
        payment->display();   // runtime polymorphism: runs the Cash/Card/UPI version
        return true;
    }
}

// ===========================================================================
// 7. Report
// ===========================================================================

void RestaurantSystem::report() const
{
    std::cout << "\n=========== SALES REPORT ===========\n";

    int pending = 0, paid = 0, cancelled = 0;
    for (int i = 0; i < orders.size(); i++) {
        switch (orders[i].getStatus()) {
            case ORDER_PENDING:   pending++;   break;
            case ORDER_PAID:      paid++;      break;
            case ORDER_CANCELLED: cancelled++; break;
        }
    }
    std::cout << "Menu items: " << menu.size() << "   Customers: " << customers.size()
              << "   Orders: " << orders.size() << "\n";
    std::cout << "Orders - paid: " << paid << ", pending: " << pending
              << ", cancelled: " << cancelled << "\n\n";

    Money gross = 0, discounts = 0, tax = 0, collected = 0, outstanding = 0;
    std::map<int, Money> spentByCustomer;
    for (int i = 0; i < bills.size(); i++) {
        const Bill& b = bills[i];
        if (b.getStatus() == BILL_PAID) {
            gross += b.getSubtotal();
            discounts += b.getDiscount();
            tax += b.getTax();
            collected += b.getTotal();
            spentByCustomer[b.getCustomerId()] += b.getTotal();
        } else if (b.getStatus() == BILL_UNPAID) {
            outstanding += b.getTotal();
        }
    }
    std::cout << "Sales before discount : " << formatMoney(gross) << "\n";
    std::cout << "Discounts given       : " << formatMoney(discounts) << "\n";
    std::cout << "Tax collected         : " << formatMoney(tax) << "\n";
    std::cout << "TOTAL COLLECTED       : " << formatMoney(collected) << "\n";
    std::cout << "Outstanding (unpaid)  : " << formatMoney(outstanding) << "\n\n";

    // Payments are stored as Payment* but each one answers getMethod() for itself.
    std::map<std::string, Money> byMethod;
    std::map<std::string, int> countByMethod;
    for (int i = 0; i < payments.size(); i++) {
        byMethod[payments[i]->getMethod()] += payments[i]->getAmount();
        countByMethod[payments[i]->getMethod()]++;
    }
    std::cout << "Collected by payment method:\n";
    if (byMethod.empty()) {
        std::cout << "  (no payments yet)\n";
    }
    for (std::map<std::string, Money>::const_iterator it = byMethod.begin(); it != byMethod.end(); ++it) {
        std::cout << "  " << it->first << ": " << formatMoney(it->second)
                  << " (" << countByMethod[it->first] << " payment(s))\n";
    }

    std::map<std::string, int> quantityByItem;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getStatus() != ORDER_PAID) continue;
        for (int j = 0; j < orders[i].getLineCount(); j++) {
            quantityByItem[orders[i].getLine(j).itemName] += orders[i].getLine(j).quantity;
        }
    }
    std::cout << "\nItems sold (paid orders):\n";
    if (quantityByItem.empty()) {
        std::cout << "  (none yet)\n";
    }
    std::string bestItem = "";
    int bestQuantity = 0;
    for (std::map<std::string, int>::const_iterator it = quantityByItem.begin(); it != quantityByItem.end(); ++it) {
        std::cout << "  " << it->first << ": " << it->second << "\n";
        if (it->second > bestQuantity) {
            bestQuantity = it->second;
            bestItem = it->first;
        }
    }
    if (bestQuantity > 0) {
        std::cout << "Best seller: " << bestItem << " (" << bestQuantity << " sold)\n";
    }

    int topCustomer = 0;
    Money topSpent = 0;
    for (std::map<int, Money>::const_iterator it = spentByCustomer.begin(); it != spentByCustomer.end(); ++it) {
        if (it->second > topSpent) {
            topSpent = it->second;
            topCustomer = it->first;
        }
    }
    if (topCustomer != 0) {
        std::cout << "Top customer: " << customerName(topCustomer) << " (" << formatMoney(topSpent) << ")\n";
    }
    std::cout << "====================================\n";
}

// ===========================================================================
// 8. Save / Load  and  9. Exit
// ===========================================================================

void RestaurantSystem::saveLoadMenu()
{
    std::cout << "\n--- Save / Load ---\n"
              << "1. Save all records to the data files now\n"
              << "2. Reload records from the data files (discards unsaved changes)\n"
              << "0. Back\n";
    int choice = readInt("Choose: ", 0, 2);
    if (choice == 1) {
        if (loadFailed && !readYesNo("The data files could not be read at start-up. Overwrite them? (y/n): ")) {
            std::cout << "Not saved.\n";
            return;
        }
        saveWithMessage();
    } else if (choice == 2) {
        if (dirty && !readYesNo("You have unsaved changes that will be lost. Reload anyway? (y/n): ")) {
            std::cout << "Reload cancelled.\n";
            return;
        }
        loadAll();
    }
}

bool RestaurantSystem::exitProgram()
{
    if (loadFailed && !readYesNo("The data files could not be read at start-up. Overwrite them with the "
                                 "current data when exiting? (y/n): ")) {
        std::cout << "Exiting without saving. The data files were not changed.\n";
        return true;
    }
    if (saveWithMessage()) {
        std::cout << "Goodbye!\n";
        return true;
    }
    return readYesNo("Saving failed. Exit anyway and lose unsaved changes? (y/n): ");
}

void RestaurantSystem::autoSave()
{
    if (loadFailed) {
        std::cout << "Not saving automatically because the data files could not be read at start-up.\n";
        return;
    }
    saveWithMessage();
}

bool RestaurantSystem::saveWithMessage()
{
    try {
        saveFiles();
        dirty = false;
        loadFailed = false;
        std::cout << "Saved " << menu.size() << " menu items, " << customers.size() << " customers, "
                  << orders.size() << " orders, " << bills.size() << " bills, "
                  << payments.size() << " payments to " << files.getDataDir() << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "SAVE FAILED: " << e.what() << "\n";
        return false;
    }
}

void RestaurantSystem::saveFiles() const
{
    std::vector<std::string> lines;

    for (int i = 0; i < menu.size(); i++) lines.push_back(menu[i].toLine());
    files.writeLines("menu.txt", lines);

    lines.clear();
    for (int i = 0; i < customers.size(); i++) lines.push_back(customers[i].toLine());
    files.writeLines("customers.txt", lines);

    lines.clear();
    for (int i = 0; i < orders.size(); i++) lines.push_back(orders[i].toLine());
    files.writeLines("orders.txt", lines);

    lines.clear();
    for (int i = 0; i < orders.size(); i++) {
        for (int j = 0; j < orders[i].getLineCount(); j++) lines.push_back(orders[i].lineRecord(j));
    }
    files.writeLines("order_items.txt", lines);

    lines.clear();
    for (int i = 0; i < bills.size(); i++) lines.push_back(bills[i].toLine());
    files.writeLines("bills.txt", lines);

    lines.clear();
    for (int i = 0; i < payments.size(); i++) lines.push_back(payments[i]->toLine());
    files.writeLines("payments.txt", lines);

    // The "next ID" counters are saved too, so an ID is never reused.
    lines.clear();
    lines.push_back(std::to_string(nextMenuId) + "|" + std::to_string(nextCustomerId) + "|" +
                    std::to_string(nextOrderId) + "|" + std::to_string(nextBillId) + "|" +
                    std::to_string(nextPaymentId));
    files.writeLines("meta.txt", lines);
}

void RestaurantSystem::clearPayments()
{
    for (int i = 0; i < payments.size(); i++) {
        delete payments[i];   // virtual destructor runs the right cleanup
    }
    payments.clear();
}

void RestaurantSystem::clearAll()
{
    menu.clear();
    customers.clear();
    orders.clear();
    bills.clear();
    clearPayments();
    nextMenuId = nextCustomerId = nextOrderId = nextBillId = nextPaymentId = 1;
}

void RestaurantSystem::loadAll()
{
    clearAll();
    loadFailed = false;
    dirty = false;

    if (!files.isReady()) {
        std::cout << "WARNING: Data folder not found. Create a folder named 'Data' next to the\n"
                     "program (or next to the Executable folder). Saving will not work until you do.\n";
        return;
    }
    try {
        loadFromFiles();
        std::cout << "Loaded " << menu.size() << " menu items, " << customers.size() << " customers, "
                  << orders.size() << " orders, " << bills.size() << " bills, "
                  << payments.size() << " payments from " << files.getDataDir() << "\n";
    } catch (const std::exception& e) {
        clearAll();
        loadFailed = true;
        std::cout << "ERROR while loading data: " << e.what() << "\n"
                  << "Starting with empty data. The files were NOT modified. Fix or delete the file,\n"
                  << "then use Save/Load > Reload.\n";
    }
}

static std::runtime_error loadError(const std::string& file, int lineNumber, const std::string& why)
{
    return std::runtime_error(file + " line " + std::to_string(lineNumber) + ": " + why);
}

void RestaurantSystem::loadFromFiles()
{
    std::vector<std::string> lines;

    // --- ID counters ---
    if (files.readLines("meta.txt", lines) && !lines.empty()) {
        std::vector<std::string> f = splitLine(lines[0], '|');
        if (f.size() != 5 || !parseInt(f[0], nextMenuId) || !parseInt(f[1], nextCustomerId) ||
            !parseInt(f[2], nextOrderId) || !parseInt(f[3], nextBillId) || !parseInt(f[4], nextPaymentId) ||
            nextMenuId < 1 || nextCustomerId < 1 || nextOrderId < 1 || nextBillId < 1 || nextPaymentId < 1) {
            throw loadError("meta.txt", 1, "invalid ID counters");
        }
    }

    // --- menu items ---
    if (files.readLines("menu.txt", lines)) {
        for (int i = 0; i < static_cast<int>(lines.size()); i++) {
            try {
                MenuItem item = MenuItem::fromLine(lines[i]);
                if (findMenu(item.getId()) >= 0) throw std::runtime_error("duplicate ID");
                menu.add(item);
                if (item.getId() >= nextMenuId) nextMenuId = item.getId() + 1;
            } catch (const std::exception& e) {
                throw loadError("menu.txt", i + 1, e.what());
            }
        }
    }

    // --- customers ---
    if (files.readLines("customers.txt", lines)) {
        for (int i = 0; i < static_cast<int>(lines.size()); i++) {
            try {
                Customer c = Customer::fromLine(lines[i]);
                if (findCustomer(c.getId()) >= 0) throw std::runtime_error("duplicate ID");
                customers.add(c);
                if (c.getId() >= nextCustomerId) nextCustomerId = c.getId() + 1;
            } catch (const std::exception& e) {
                throw loadError("customers.txt", i + 1, e.what());
            }
        }
    }

    // --- orders (headers) ---
    if (files.readLines("orders.txt", lines)) {
        for (int i = 0; i < static_cast<int>(lines.size()); i++) {
            try {
                Order o = Order::fromLine(lines[i]);
                if (findOrder(o.getId()) >= 0) throw std::runtime_error("duplicate ID");
                if (findCustomer(o.getCustomerId()) < 0) throw std::runtime_error("refers to an unknown customer");
                orders.add(o);
                if (o.getId() >= nextOrderId) nextOrderId = o.getId() + 1;
            } catch (const std::exception& e) {
                throw loadError("orders.txt", i + 1, e.what());
            }
        }
    }

    // --- order items (attached to their orders) ---
    if (files.readLines("order_items.txt", lines)) {
        for (int i = 0; i < static_cast<int>(lines.size()); i++) {
            try {
                int orderId = Order::orderIdOfLineRecord(lines[i]);
                int idx = findOrder(orderId);
                if (idx < 0) throw std::runtime_error("refers to an unknown order");
                orders[idx].addLineFromRecord(lines[i]);
            } catch (const std::exception& e) {
                throw loadError("order_items.txt", i + 1, e.what());
            }
        }
    }

    // --- bills ---
    if (files.readLines("bills.txt", lines)) {
        for (int i = 0; i < static_cast<int>(lines.size()); i++) {
            try {
                Bill b = Bill::fromLine(lines[i]);
                if (findBill(b.getId()) >= 0) throw std::runtime_error("duplicate ID");
                if (findOrder(b.getOrderId()) < 0) throw std::runtime_error("refers to an unknown order");
                bills.add(b);
                if (b.getId() >= nextBillId) nextBillId = b.getId() + 1;
            } catch (const std::exception& e) {
                throw loadError("bills.txt", i + 1, e.what());
            }
        }
    }

    // --- payments (rebuilt as the correct derived type) ---
    if (files.readLines("payments.txt", lines)) {
        for (int i = 0; i < static_cast<int>(lines.size()); i++) {
            try {
                Payment* p = paymentFromLine(lines[i]);
                bool bad = (findBill(p->getBillId()) < 0);
                for (int j = 0; j < payments.size(); j++) {
                    if (payments[j]->getId() == p->getId()) bad = true;
                }
                if (bad) {
                    delete p;
                    throw std::runtime_error("duplicate ID or unknown bill");
                }
                try {
                    payments.add(p);
                } catch (...) {
                    delete p;
                    throw;
                }
                if (p->getId() >= nextPaymentId) nextPaymentId = p->getId() + 1;
            } catch (const std::exception& e) {
                throw loadError("payments.txt", i + 1, e.what());
            }
        }
    }
}

// A tiny "factory": reads the METHOD field and creates the matching derived
// object on the heap. The CALLER owns the returned pointer.
Payment* RestaurantSystem::paymentFromLine(const std::string& line) const
{
    std::vector<std::string> f = splitLine(line, '|');
    if (f.size() != 5) {
        throw std::runtime_error("payment record should have 5 fields");
    }
    int id = 0, billId = 0;
    long long amount = 0;
    if (!parseInt(f[0], id) || !parseInt(f[1], billId) || !parseInteger(f[2], amount)) {
        throw std::runtime_error("payment record has an invalid number");
    }
    if (f[3] == "CASH") {
        long long tendered = 0;
        if (!parseInteger(f[4], tendered)) throw std::runtime_error("invalid cash amount");
        return new CashPayment(id, billId, amount, tendered);
    }
    if (f[3] == "CARD") {
        return new CardPayment(id, billId, amount, f[4]);
    }
    if (f[3] == "UPI") {
        return new UpiPayment(id, billId, amount, f[4]);
    }
    throw std::runtime_error("unknown payment method '" + f[3] + "'");
}
