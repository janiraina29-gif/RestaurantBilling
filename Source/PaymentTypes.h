#ifndef PAYMENTTYPES_H
#define PAYMENTTYPES_H

#include <string>
#include "Payment.h"

// The three concrete payment methods. Each one validates its own data in its
// constructor (throws std::invalid_argument) and overrides the virtual functions.

class CashPayment : public Payment {
private:
    Money tendered;   // cash handed over by the customer
public:
    CashPayment(int id, int billId, Money amount, Money tendered);
    Money getChange() const;
    std::string getMethod() const override;
    std::string getDetails() const override;
    std::string getStoredValue() const override;
};

class CardPayment : public Payment {
private:
    std::string last4;   // only the last 4 digits are ever stored
public:
    CardPayment(int id, int billId, Money amount, const std::string& last4);
    std::string getMethod() const override;
    std::string getDetails() const override;
    std::string getStoredValue() const override;
};

class UpiPayment : public Payment {
private:
    std::string upiId;   // like name@bank
public:
    UpiPayment(int id, int billId, Money amount, const std::string& upiId);
    std::string getMethod() const override;
    std::string getDetails() const override;
    std::string getStoredValue() const override;
};

#endif
