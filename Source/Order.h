#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "DynamicArray.h"
#include "Utils.h"

enum OrderStatus { ORDER_PENDING, ORDER_PAID, ORDER_CANCELLED };

// One line of an order. The item NAME and PRICE are copied from the menu at
// the moment of ordering, so later menu edits never change old orders/bills.
struct OrderLine {
    int menuItemId;
    std::string itemName;
    Money unitPrice;
    int quantity;

    OrderLine() : menuItemId(0), itemName(""), unitPrice(0), quantity(0) {}
    Money lineTotal() const { return unitPrice * quantity; }
};

class Order {
private:
    int id;
    int customerId;                 // link to a Customer by ID (not by pointer)
    OrderStatus status;
    DynamicArray<OrderLine> lines;  // dynamic memory owned by this Order

public:
    Order();
    Order(int id, int customerId);

    int getId() const;
    int getCustomerId() const;
    OrderStatus getStatus() const;
    std::string getStatusText() const;
    void setStatus(OrderStatus newStatus);

    int getLineCount() const;
    const OrderLine& getLine(int index) const;

    // Adds a line, or increases the quantity if the item is already there.
    // Throws std::invalid_argument for bad quantity (1-99) or price.
    void addLine(int menuItemId, const std::string& itemName, Money unitPrice, int quantity);

    Money getSubtotal() const;
    void display(const std::string& customerName) const;

    // Header file format:  id|customerId|STATUS
    std::string toLine() const;
    static Order fromLine(const std::string& line);

    // Item lines are stored in a separate file:
    //   orderId|menuItemId|itemName|unitPricePaise|quantity
    std::string lineRecord(int index) const;
    static int orderIdOfLineRecord(const std::string& record);
    void addLineFromRecord(const std::string& record);
};

#endif
