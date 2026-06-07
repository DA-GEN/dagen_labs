#ifndef POTION_HPP
#define POTION_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <string>

class Potion : public Item {
private:
    int heal_amount_;

public:
    Potion(const std::string& name, const std::string& description, int heal_amount)
        : Item(name, description), heal_amount_(heal_amount) {}

    int get_heal_amount() const { return heal_amount_; }

    std::string use(Character* character) override {
        if (character == nullptr) {
            return "Помилка: Невалідний персонаж!";
        }

        std::string heal_log = character->heal(heal_amount_);

        // ВИПРАВЛЕНО ТУТ: замінили << на +
        return "🧪 " + character->get_name() + " використовує " + name_ + "! " + heal_log;
    }

    std::string get_info_string() const override {
        return Item::get_info_string() + " (Відновлення: " + std::to_string(heal_amount_) + " HP)";
    }
};

#endif // POTION_HPP
