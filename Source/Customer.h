#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Utils.h"

// A person who places orders.
class Customer {
private:
    int id;
    std::string name;
    std::string phone;   // exactly 10 digits

public:
    Customer();
    Customer(int id, const std::string& name, const std::string& phone);

    int getId() const;
    const std::string& getName() const;
    const std::string& getPhone() const;

    void setName(const std::string& newName);
    void setPhone(const std::string& newPhone);

    void display() const;
    static void printHeader();

    // File format:  id|name|phone
    std::string toLine() const;
    static Customer fromLine(const std::string& line);
};

#endif
