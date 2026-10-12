#ifndef BILL_H
#define BILL_H

#include <string>
#include "Utils.h"

enum BillStatus { BILL_UNPAID, BILL_PAID, BILL_CANCELLED };

// The invoice for one Order.
//   discount = subtotal * discountPercent%
//   tax      = (subtotal - discount) * 5%
//   total    = subtotal - discount + tax
// All values are stored at creation, so changing the tax rate later can never
// alter old bills.
class Bill {
private:
    int id;
    int orderId;
    int customerId;
    Money subtotal;
    int discountPercent;
    Money discount;
    Money tax;
    Money total;
    BillStatus status;
    std::string date;

public:
    static constexpr int TAX_PERCENT = 5;

    Bill();
    Bill(int id, int orderId, int customerId, Money subtotal,
         int discountPercent, const std::string& date);

    int getId() const;
    int getOrderId() const;
    int getCustomerId() const;
    Money getSubtotal() const;
    Money getDiscount() const;
    Money getTax() const;
    Money getTotal() const;
    BillStatus getStatus() const;
    std::string getStatusText() const;

    // Throw std::logic_error if the status change is not allowed.
    void markPaid();
    void markCancelled();

    void display() const;        // full bill breakdown
    static void printHeader();   // header for the one-line summary table
    void displayRow() const;     // one-line summary

    // File format:
    //   id|orderId|customerId|subtotal|discountPercent|discount|tax|total|STATUS|date
    std::string toLine() const;
    static Bill fromLine(const std::string& line);
};

#endif
