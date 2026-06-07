#ifndef ARMOR_HPP
#define ARMOR_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <string>
#include <sstream>

class Armor : public Item {
private:
    int defense_;

public:
    Armor(const std::string& name, const std::string& description, int defense)
        : Item(name, description), defense_(defense) {
    }

    int get_defense() const { return defense_; }

    // ЗМІНА: Повертає рядок замість void
    std::string use(Character* character) override {
        if (character == nullptr) {
            return "Помилка: Невалідний персонаж!";
        }

        character->modify_defense(defense_);

        std::ostringstream ss;
        ss << character->get_name() << " екіпірує " << name_
            << ". Захист збільшено на " << defense_ << "!";
        return ss.str();
    }

    // ЗМІНА: Повертає рядок для відображення в UI
    std::string get_info_string() const override {
        return Item::get_info_string() + " (Захист: " + std::to_string(defense_) + ")";
    }

    // display_info видалено або можна залишити як deprecated, 
    // але краще використовувати get_info_string
};

#endif // ARMOR_HPP