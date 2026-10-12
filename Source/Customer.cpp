#include "Customer.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

Customer::Customer()
    : id(0), name(""), phone("")
{
}

Customer::Customer(int id, const std::string& name, const std::string& phone)
    : id(id), name(""), phone("")
{
    setName(name);
    setPhone(phone);
}

int Customer::getId() const { return id; }
const std::string& Customer::getName() const { return name; }
const std::string& Customer::getPhone() const { return phone; }

void Customer::setName(const std::string& newName)
{
    std::string clean = trim(newName);
    requireSafeText(clean, "Customer name");
    name = clean;
}

void Customer::setPhone(const std::string& newPhone)
{
    std::string clean = trim(newPhone);
    if (!isAllDigits(clean) || clean.size() != 10) {
        throw std::invalid_argument("Phone number must be exactly 10 digits.");
    }
    phone = clean;
}

void Customer::printHeader()
{
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(26) << "Name"
              << "Phone" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
}

void Customer::display() const
{
    std::cout << std::left
              << std::setw(5)  << id
              << std::setw(26) << name
              << phone << std::endl;
}

std::string Customer::toLine() const
{
    return std::to_string(id) + "|" + name + "|" + phone;
}

Customer Customer::fromLine(const std::string& line)
{
    std::vector<std::string> f = splitLine(line, '|');
    if (f.size() != 3) {
        throw std::runtime_error("customer record should have 3 fields");
    }
    int id = 0;
    if (!parseInt(f[0], id) || id <= 0) {
        throw std::runtime_error("customer record has an invalid id");
    }
    return Customer(id, f[1], f[2]);
}
