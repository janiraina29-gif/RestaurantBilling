#include "PaymentTypes.h"

#include <stdexcept>

// ---------------- CashPayment ----------------

CashPayment::CashPayment(int id, int billId, Money amount, Money tendered)
    : Payment(id, billId, amount), tendered(tendered)
{
    if (tendered < amount) {
        throw std::invalid_argument("Cash handed over (" + formatMoney(tendered) +
                                    ") is less than the amount due (" + formatMoney(amount) + ").");
    }
}

Money CashPayment::getChange() const
{
    return tendered - amount;
}

std::string CashPayment::getMethod() const { return "CASH"; }

std::string CashPayment::getDetails() const
{
    return "Cash tendered " + formatMoney(tendered) + ", change returned " + formatMoney(getChange());
}

std::string CashPayment::getStoredValue() const
{
    return std::to_string(tendered);
}

// ---------------- CardPayment ----------------

CardPayment::CardPayment(int id, int billId, Money amount, const std::string& last4)
    : Payment(id, billId, amount), last4(trim(last4))
{
    if (!isAllDigits(this->last4) || this->last4.size() != 4) {
        throw std::invalid_argument("Enter exactly the last 4 digits of the card.");
    }
}

std::string CardPayment::getMethod() const { return "CARD"; }

std::string CardPayment::getDetails() const
{
    return "Card ending " + last4;
}

std::string CardPayment::getStoredValue() const
{
    return last4;
}

// ---------------- UpiPayment ----------------

UpiPayment::UpiPayment(int id, int billId, Money amount, const std::string& upiId)
    : Payment(id, billId, amount), upiId(trim(upiId))
{
    requireSafeText(this->upiId, "UPI ID");
    size_t at = this->upiId.find('@');
    bool oneAt = (at != std::string::npos) && (this->upiId.find('@', at + 1) == std::string::npos);
    bool bothSides = (at != 0) && (at + 1 < this->upiId.size());
    bool noSpaces = this->upiId.find(' ') == std::string::npos;
    if (!oneAt || !bothSides || !noSpaces) {
        throw std::invalid_argument("UPI ID must look like name@bank (no spaces).");
    }
}

std::string UpiPayment::getMethod() const { return "UPI"; }

std::string UpiPayment::getDetails() const
{
    return "UPI ID " + upiId;
}

std::string UpiPayment::getStoredValue() const
{
    return upiId;
}
