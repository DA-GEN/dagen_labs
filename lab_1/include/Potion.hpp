#ifndef POTION_HPP
#define POTION_HPP

#include "Item.hpp"
#include "Character.hpp"
#include <iostream>

/**
 * @brief Potion item that restores HP
 */
class Potion : public Item {
private:
    int heal_amount_;  ///< Amount of HP restored

public:
    /**
     * @brief Construct a new Potion
     * @param name Potion name
     * @param description Potion description
     * @param heal_amount HP restoration amount
     */
    Potion(const std::string& name, const std::string& description, int heal_amount)
        : Item(name, description), heal_amount_(heal_amount) {}

    /** @brief Get heal amount */
    int get_heal_amount() const { return heal_amount_; }

    void use(Character* character) override {
        if (character == nullptr) {
            std::cout << "Невалідний персонаж!" << std::endl;
            return;
        }

        std::cout << character->get_name() << " використовує " << name_ << "!" << std::endl;
        character->heal(heal_amount_);
    }

    void display_info() const override {
        Item::display_info();
        std::cout << "Відновлення: " << heal_amount_ << " HP" << std::endl;
    }
};

#endif // POTION_HPP

