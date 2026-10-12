#include "MenuItem.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

MenuItem::MenuItem()
    : id(0), name(""), category(""), price(0), available(true)
{
}

MenuItem::MenuItem(int id, const std::string& name, const std::string& category,
                   Money price, bool available)
    : id(id), name(""), category(""), price(0), available(available)
{
    // Go through the setters so the same validation rules apply everywhere.
    setName(name);
    setCategory(category);
    setPrice(price);
}

int MenuItem::getId() const { return id; }
const std::string& MenuItem::getName() const { return name; }
const std::string& MenuItem::getCategory() const { return category; }
Money MenuItem::getPrice() const { return price; }
bool MenuItem::isAvailable() const { return available; }

void MenuItem::setName(const std::string& newName)
{
    std::string clean = trim(newName);
    requireSafeText(clean, "Menu item name");
    name = clean;
}

void MenuItem::setCategory(const std::string& newCategory)
{
    std::string clean = trim(newCategory);
    requireSafeText(clean, "Menu item category");
    category = clean;
}

void MenuItem::setPrice(Money newPrice)
{
    if (newPrice <= 0) {
        throw std::invalid_argument("Menu item price must be greater than zero.");
    }
    price = newPrice;
}

void MenuItem::setAvailable(bool value)
{
    available = value;
}

void MenuItem::printHeader()
{
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(26) << "Name"
              << std::setw(14) << "Category"
              << std::setw(13) << "Price"
              << "Available" << std::endl;
    std::cout << std::string(67, '-') << std::endl;
}

void MenuItem::display() const
{
    std::cout << std::left
              << std::setw(5)  << id
              << std::setw(26) << name
              << std::setw(14) << category
              << std::setw(13) << formatMoney(price)
              << (available ? "Yes" : "No")
              << std::endl;
}

std::string MenuItem::toLine() const
{
    return std::to_string(id) + "|" + name + "|" + category + "|" +
           std::to_string(price) + "|" + (available ? "1" : "0");
}

MenuItem MenuItem::fromLine(const std::string& line)
{
    std::vector<std::string> f = splitLine(line, '|');
    if (f.size() != 5) {
        throw std::runtime_error("menu item record should have 5 fields");
    }
    int id = 0;
    long long price = 0;
    if (!parseInt(f[0], id) || id <= 0 || !parseInteger(f[3], price) ||
        (f[4] != "0" && f[4] != "1")) {
        throw std::runtime_error("menu item record has an invalid number");
    }
    return MenuItem(id, f[1], f[2], price, f[4] == "1");
}
