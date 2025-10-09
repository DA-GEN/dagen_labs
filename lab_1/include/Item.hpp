#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <iostream>

class Character;

class Item {
protected:
    std::string name_;
    std::string description_;

public:
    Item(const std::string& name, const std::string& description)
        : name_(name), description_(description) {}

    virtual ~Item() = default;

    virtual void use(Character* character) = 0;

    std::string get_name() const { return name_; }
    std::string get_description() const { return description_; }

    virtual void display_info() const {
        std::cout << "Предмет: " << name_ << std::endl;
        std::cout << "Опис: " << description_ << std::endl;
    }
};

#endif // ITEM_HPP

