#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

class Character; // Forward declaration

class Item {
protected:
    std::string name_;
    std::string description_;

public:
    Item(const std::string& name, const std::string& description)
        : name_(name), description_(description) {
    }

    virtual ~Item() = default;

    // ЗМІНА: Повертає опис результату використання
    virtual std::string use(Character* character) = 0;

    std::string get_name() const { return name_; }
    std::string get_description() const { return description_; }

    // ЗМІНА: Повертає рядок замість друку в консоль
    virtual std::string get_info_string() const {
        return name_ + ": " + description_;
    }
};

#endif // ITEM_HPP