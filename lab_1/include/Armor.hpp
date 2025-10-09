#ifndef ARMOR_HPP
#define ARMOR_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <iostream>

class Armor : public Item {
private:
    int defense_;

public:
    Armor(const std::string& name, const std::string& description, int defense)
        : Item(name, description), defense_(defense) {}

    int get_defense() const { return defense_; }

    void use(Character* character) override {
        if (character == nullptr) {
            std::cout << "Невалідний персонаж!" << std::endl;
            return;
        }

        std::cout << character->get_name() << " екіпірує " << name_ 
                  << " (Захист: " << defense_ << ")" << std::endl;
        
        character->modify_defense(defense_);
        std::cout << "Захист збільшено на " << defense_ << "!" << std::endl;
    }

    void display_info() const override {
        Item::display_info();
        std::cout << "Захист: " << defense_ << std::endl;
    }
};

#endif // ARMOR_HPP

