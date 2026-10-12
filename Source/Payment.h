#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include "Utils.h"

// ABSTRACT base class for every way of paying a bill.
// It contains pure virtual functions (= 0), so you cannot create a plain
// Payment object; you create CashPayment, CardPayment or UpiPayment and talk
// to them through a Payment* pointer. The call p->getDetails() runs the
// version belonging to the real object type: runtime polymorphism.
class Payment {
protected:
    int id;
    int billId;     // link to the Bill by ID
    Money amount;   // amount paid, in paise

public:
    Payment();
    Payment(int id, int billId, Money amount);

    // VIRTUAL destructor: deleting a derived object through a Payment*
    // must run the derived destructor too. Without 'virtual' it is undefined behaviour.
    virtual ~Payment();

    int getId() const;
    int getBillId() const;
    Money getAmount() const;

    // Each payment type must supply these three:
    virtual std::string getMethod() const = 0;        // "CASH", "CARD" or "UPI"
    virtual std::string getDetails() const = 0;       // text shown to the user
    virtual std::string getStoredValue() const = 0;   // text saved in the file

    void display() const;       // not virtual: it calls the virtual functions above
    static void printHeader();
    void displayRow() const;

    // File format:  id|billId|amountPaise|METHOD|storedValue
    std::string toLine() const;
};

#endif
