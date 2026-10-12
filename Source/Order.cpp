#include "Order.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

static const int MAX_QUANTITY = 99;

Order::Order()
    : id(0), customerId(0), status(ORDER_PENDING), lines()
{
}

Order::Order(int id, int customerId)
    : id(id), customerId(customerId), status(ORDER_PENDING), lines()
{
    if (id <= 0 || customerId <= 0) {
        throw std::invalid_argument("Order and customer IDs must be positive.");
    }
}

int Order::getId() const { return id; }
int Order::getCustomerId() const { return customerId; }
OrderStatus Order::getStatus() const { return status; }
void Order::setStatus(OrderStatus newStatus) { status = newStatus; }

std::string Order::getStatusText() const
{
    switch (status) {
        case ORDER_PENDING:   return "PENDING";
        case ORDER_PAID:      return "PAID";
        case ORDER_CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

int Order::getLineCount() const { return lines.size(); }

const OrderLine& Order::getLine(int index) const { return lines[index]; }

void Order::addLine(int menuItemId, const std::string& itemName, Money unitPrice, int quantity)
{
    requireSafeText(itemName, "Item name");
    if (unitPrice <= 0) {
        throw std::invalid_argument("Unit price must be greater than zero.");
    }
    if (quantity < 1 || quantity > MAX_QUANTITY) {
        throw std::invalid_argument("Quantity must be between 1 and 99.");
    }

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].menuItemId == menuItemId) {
            int newQuantity = lines[i].quantity + quantity;
            if (newQuantity > MAX_QUANTITY) {
                throw std::invalid_argument("Total quantity of one item cannot exceed 99.");
            }
            lines[i].quantity = newQuantity;
            return;
        }
    }

    OrderLine line;
    line.menuItemId = menuItemId;
    line.itemName = itemName;
    line.unitPrice = unitPrice;
    line.quantity = quantity;
    lines.add(line);
}

Money Order::getSubtotal() const
{
    Money total = 0;
    for (int i = 0; i < lines.size(); i++) {
        total += lines[i].lineTotal();
    }
    return total;
}

void Order::display(const std::string& customerName) const
{
    std::cout << "Order #" << id << "   Customer: " << customerName
              << " (#" << customerId << ")   Status: " << getStatusText() << std::endl;
    std::cout << "   " << std::left << std::setw(26) << "Item"
              << std::setw(6) << "Qty"
              << std::setw(14) << "Unit price"
              << "Line total" << std::endl;
    for (int i = 0; i < lines.size(); i++) {
        std::cout << "   " << std::left << std::setw(26) << lines[i].itemName
                  << std::setw(6) << lines[i].quantity
                  << std::setw(14) << formatMoney(lines[i].unitPrice)
                  << formatMoney(lines[i].lineTotal()) << std::endl;
    }
    std::cout << "   Subtotal: " << formatMoney(getSubtotal()) << std::endl;
}

std::string Order::toLine() const
{
    return std::to_string(id) + "|" + std::to_string(customerId) + "|" + getStatusText();
}

Order Order::fromLine(const std::string& line)
{
    std::vector<std::string> f = splitLine(line, '|');
    if (f.size() != 3) {
        throw std::runtime_error("order record should have 3 fields");
    }
    int id = 0;
    int customerId = 0;
    if (!parseInt(f[0], id) || !parseInt(f[1], customerId) || id <= 0 || customerId <= 0) {
        throw std::runtime_error("order record has an invalid number");
    }
    Order order(id, customerId);
    if (f[2] == "PENDING") {
        order.status = ORDER_PENDING;
    } else if (f[2] == "PAID") {
        order.status = ORDER_PAID;
    } else if (f[2] == "CANCELLED") {
        order.status = ORDER_CANCELLED;
    } else {
        throw std::runtime_error("order record has an unknown status '" + f[2] + "'");
    }
    return order;
}

std::string Order::lineRecord(int index) const
{
    const OrderLine& l = lines[index];
    return std::to_string(id) + "|" + std::to_string(l.menuItemId) + "|" + l.itemName + "|" +
           std::to_string(l.unitPrice) + "|" + std::to_string(l.quantity);
}

int Order::orderIdOfLineRecord(const std::string& record)
{
    std::vector<std::string> f = splitLine(record, '|');
    int orderId = 0;
    if (f.size() != 5 || !parseInt(f[0], orderId)) {
        throw std::runtime_error("order item record is malformed");
    }
    return orderId;
}

void Order::addLineFromRecord(const std::string& record)
{
    std::vector<std::string> f = splitLine(record, '|');
    int orderId = 0;
    int menuItemId = 0;
    int quantity = 0;
    long long price = 0;
    if (f.size() != 5 || !parseInt(f[0], orderId) || !parseInt(f[1], menuItemId) ||
        !parseInteger(f[3], price) || !parseInt(f[4], quantity)) {
        throw std::runtime_error("order item record is malformed");
    }
    if (orderId != id) {
        throw std::runtime_error("order item record belongs to a different order");
    }
    addLine(menuItemId, f[2], price, quantity);
}
