#ifndef POTION_HPP
#define POTION_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <string>
#include <sstream>

class Potion : public Item {
private:
    int heal_amount_;

public:
    Potion(const std::string& name, const std::string& description, int heal_amount)
        : Item(name, description), heal_amount_(heal_amount) {
    }

    int get_heal_amount() const { return heal_amount_; }

    std::string use(Character* character) override {
        if (character == nullptr) {
            return "Помилка: Невалідний персонаж!";
        }

        // Викликаємо heal, який повертає рядок (ми його змінили в Character.hpp)
        std::string heal_log = character->heal(heal_amount_);

        return "🧪 " + character->get_name() << " використовує " << name_ << "! " + heal_log;
    }

    std::string get_info_string() const override {
        return Item::get_info_string() + " (Відновлення: " + std::to_string(heal_amount_) + " HP)";
    }
};

#endif // POTION_HPP