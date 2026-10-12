#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include "Utils.h"

// One dish/drink on the restaurant menu.
// Data members are private (encapsulation): the only way to change them is
// through the setters below, and the setters reject invalid values.
class MenuItem {
private:
    int id;
    std::string name;
    std::string category;
    Money price;      // in paise
    bool available;   // false = cannot be added to new orders (soft delete)

public:
    // Default constructor: needed so we can write new MenuItem[capacity].
    MenuItem();

    // Parameterized constructor: validates its arguments, throws
    // std::invalid_argument if the name/category is empty or the price is not positive.
    MenuItem(int id, const std::string& name, const std::string& category,
             Money price, bool available = true);

    int getId() const;
    const std::string& getName() const;
    const std::string& getCategory() const;
    Money getPrice() const;
    bool isAvailable() const;

    void setName(const std::string& newName);
    void setCategory(const std::string& newCategory);
    void setPrice(Money newPrice);
    void setAvailable(bool value);

    void display() const;          // one table row
    static void printHeader();     // the matching table header

    // File format:  id|name|category|pricePaise|available(1/0)
    std::string toLine() const;
    static MenuItem fromLine(const std::string& line);   // throws on bad data
};

#endif
