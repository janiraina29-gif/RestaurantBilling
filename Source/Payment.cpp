#include "Payment.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

Payment::Payment()
    : id(0), billId(0), amount(0)
{
}

Payment::Payment(int id, int billId, Money amount)
    : id(id), billId(billId), amount(amount)
{
    if (id <= 0 || billId <= 0) {
        throw std::invalid_argument("Payment and bill IDs must be positive.");
    }
    if (amount <= 0) {
        throw std::invalid_argument("Payment amount must be greater than zero.");
    }
}

Payment::~Payment()
{
}

int Payment::getId() const { return id; }
int Payment::getBillId() const { return billId; }
Money Payment::getAmount() const { return amount; }

void Payment::display() const
{
    std::cout << "Payment #" << id << "   Bill #" << billId
              << "   Method: " << getMethod()
              << "   Amount: " << formatMoney(amount) << std::endl;
    std::cout << "   " << getDetails() << std::endl;
}

void Payment::printHeader()
{
    std::cout << std::left
              << std::setw(9)  << "Payment"
              << std::setw(6)  << "Bill"
              << std::setw(8)  << "Method"
              << std::setw(14) << "Amount"
              << "Details" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
}

void Payment::displayRow() const
{
    std::cout << std::left
              << std::setw(9)  << id
              << std::setw(6)  << billId
              << std::setw(8)  << getMethod()
              << std::setw(14) << formatMoney(amount)
              << getDetails() << std::endl;
}

std::string Payment::toLine() const
{
    return std::to_string(id) + "|" + std::to_string(billId) + "|" +
           std::to_string(amount) + "|" + getMethod() + "|" + getStoredValue();
}
