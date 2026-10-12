#include "Bill.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

// Integer percentage, rounded half up (e.g. 12.5 paise -> 13 paise).
static Money percentOf(Money amount, int percent)
{
    return (amount * percent + 50) / 100;
}

Bill::Bill()
    : id(0), orderId(0), customerId(0), subtotal(0), discountPercent(0),
      discount(0), tax(0), total(0), status(BILL_UNPAID), date("")
{
}

Bill::Bill(int id, int orderId, int customerId, Money subtotal,
           int discountPercent, const std::string& date)
    : id(id), orderId(orderId), customerId(customerId), subtotal(subtotal),
      discountPercent(discountPercent), discount(0), tax(0), total(0),
      status(BILL_UNPAID), date(date)
{
    if (id <= 0 || orderId <= 0 || customerId <= 0) {
        throw std::invalid_argument("Bill, order and customer IDs must be positive.");
    }
    if (subtotal <= 0) {
        throw std::invalid_argument("A bill must have a positive subtotal.");
    }
    if (discountPercent < 0 || discountPercent > 100) {
        throw std::invalid_argument("Discount must be between 0 and 100 percent.");
    }
    requireSafeText(date, "Date");

    discount = percentOf(subtotal, discountPercent);
    tax = percentOf(subtotal - discount, TAX_PERCENT);
    total = subtotal - discount + tax;
}

int Bill::getId() const { return id; }
int Bill::getOrderId() const { return orderId; }
int Bill::getCustomerId() const { return customerId; }
Money Bill::getSubtotal() const { return subtotal; }
Money Bill::getDiscount() const { return discount; }
Money Bill::getTax() const { return tax; }
Money Bill::getTotal() const { return total; }
BillStatus Bill::getStatus() const { return status; }

std::string Bill::getStatusText() const
{
    switch (status) {
        case BILL_UNPAID:    return "UNPAID";
        case BILL_PAID:      return "PAID";
        case BILL_CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

void Bill::markPaid()
{
    if (status != BILL_UNPAID) {
        throw std::logic_error("Only an unpaid bill can be paid.");
    }
    status = BILL_PAID;
}

void Bill::markCancelled()
{
    if (status != BILL_UNPAID) {
        throw std::logic_error("Only an unpaid bill can be cancelled.");
    }
    status = BILL_CANCELLED;
}

void Bill::display() const
{
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "BILL #" << id << "    Date: " << date << std::endl;
    std::cout << "Order #" << orderId << "    Customer #" << customerId << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::left << std::setw(22) << "Subtotal"
              << std::right << std::setw(14) << formatMoney(subtotal) << std::endl;
    std::cout << std::left << std::setw(22) << ("Discount (" + std::to_string(discountPercent) + "%)")
              << std::right << std::setw(14) << ("-" + formatMoney(discount)) << std::endl;
    std::cout << std::left << std::setw(22) << ("Tax (" + std::to_string(TAX_PERCENT) + "%)")
              << std::right << std::setw(14) << formatMoney(tax) << std::endl;
    std::cout << std::left << std::setw(22) << "TOTAL"
              << std::right << std::setw(14) << formatMoney(total) << std::endl;
    std::cout << "Status: " << getStatusText() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void Bill::printHeader()
{
    std::cout << std::left
              << std::setw(6)  << "Bill"
              << std::setw(8)  << "Order"
              << std::setw(10) << "Customer"
              << std::setw(14) << "Total"
              << std::setw(12) << "Status"
              << "Date" << std::endl;
    std::cout << std::string(64, '-') << std::endl;
}

void Bill::displayRow() const
{
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(8)  << orderId
              << std::setw(10) << customerId
              << std::setw(14) << formatMoney(total)
              << std::setw(12) << getStatusText()
              << date << std::endl;
}

std::string Bill::toLine() const
{
    return std::to_string(id) + "|" + std::to_string(orderId) + "|" +
           std::to_string(customerId) + "|" + std::to_string(subtotal) + "|" +
           std::to_string(discountPercent) + "|" + std::to_string(discount) + "|" +
           std::to_string(tax) + "|" + std::to_string(total) + "|" +
           getStatusText() + "|" + date;
}

Bill Bill::fromLine(const std::string& line)
{
    std::vector<std::string> f = splitLine(line, '|');
    if (f.size() != 10) {
        throw std::runtime_error("bill record should have 10 fields");
    }
    int id = 0, orderId = 0, customerId = 0, percent = 0;
    long long subtotal = 0, discount = 0, tax = 0, total = 0;
    if (!parseInt(f[0], id) || !parseInt(f[1], orderId) || !parseInt(f[2], customerId) ||
        !parseInteger(f[3], subtotal) || !parseInt(f[4], percent) ||
        !parseInteger(f[5], discount) || !parseInteger(f[6], tax) ||
        !parseInteger(f[7], total)) {
        throw std::runtime_error("bill record has an invalid number");
    }

    // Build with the normal constructor (validates), then restore the stored
    // amounts exactly as they were when the bill was created.
    Bill bill(id, orderId, customerId, subtotal, percent, f[9]);
    if (total != subtotal - discount + tax || discount > subtotal) {
        throw std::runtime_error("bill amounts do not add up");
    }
    bill.discount = discount;
    bill.tax = tax;
    bill.total = total;

    if (f[8] == "UNPAID") {
        bill.status = BILL_UNPAID;
    } else if (f[8] == "PAID") {
        bill.status = BILL_PAID;
    } else if (f[8] == "CANCELLED") {
        bill.status = BILL_CANCELLED;
    } else {
        throw std::runtime_error("bill record has an unknown status '" + f[8] + "'");
    }
    return bill;
}
