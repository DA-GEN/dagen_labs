#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <string>
#include <sstream>

class Weapon : public Item {
private:
    int damage_;

public:
    Weapon(const std::string& name, const std::string& description, int damage)
        : Item(name, description), damage_(damage) {
    }

    int get_damage() const { return damage_; }

    std::string use(Character* character) override {
        if (character == nullptr) {
            return "Помилка: Невалідний персонаж!";
        }

        int bonus = damage_ / 2;
        character->modify_attack_power(bonus);

        std::ostringstream ss;
        ss << "⚔️ " << character->get_name() << " екіпірує " << name_
            << ". Сила атаки збільшена на " << bonus << "!";
        return ss.str();
    }

    std::string get_info_string() const override {
        return Item::get_info_string() + " (Пошкодження зброї: " + std::to_string(damage_) + ")";
    }
};

#endif // WEAPON_HPP