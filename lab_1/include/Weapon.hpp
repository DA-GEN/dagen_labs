#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <iostream>

class Weapon : public Item {
private:
    int damage_;

public:
    Weapon(const std::string& name, const std::string& description, int damage)
        : Item(name, description), damage_(damage) {}

    int get_damage() const { return damage_; }

    void use(Character* character) override {
        if (character == nullptr) {
            std::cout << "Невалідний персонаж!" << std::endl;
            return;
        }

        std::cout << character->get_name() << " екіпірує " << name_ 
                  << " (Пошкодження: " << damage_ << ")" << std::endl;
        
        character->modify_attack_power(damage_ / 2);  // Boosts attack by half of weapon damage
        std::cout << "Сила атаки збільшена на " << (damage_ / 2) << "!" << std::endl;
    }

    void display_info() const override {
        Item::display_info();
        std::cout << "Пошкодження: " << damage_ << std::endl;
    }
};

#endif // WEAPON_HPP

