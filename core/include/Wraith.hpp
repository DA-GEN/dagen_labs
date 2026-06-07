#ifndef WRAITH_HPP
#define WRAITH_HPP

#include "Enemy.hpp"
#include <string>
#include <sstream>

class Wraith : public Enemy {
private:
    const double physical_resistance_ = 0.5;

public:
    Wraith(const std::string& name = "Wraith")
        : Enemy(name, 60, 18, 5) {
    }

    std::string attack(Character& target) override {
        // Атака + Вампіризм
        std::string damage_log = target.take_damage(attack_power_);

        int heal_amount = attack_power_ / 3;
        std::string heal_log = heal(heal_amount); // Примара лікує сама себе
        (void)heal_log;

        return "👻 " + name_ + " (Примара) використовує СПЕКТРАЛЬНЕ ВИСМОКТУВАННЯ! " +
            damage_log + " (Примара відновила сили)";
    }

    std::string get_stats_string() const override {
        return Character::get_stats_string() + " [Примара: 50% фіз. резист, вампіризм]";
    }

protected:
    // ЗМІНА: замість повного дублювання take_damage (як було раніше) тут
    // лише розширюється хук базового класу - спершу застосовується фізичний
    // резист примари, а тоді - стандартний розрахунок захисту з Character.
    // Магічна шкода (DamageType::Magical) проходить крізь резист незмінною,
    // адже спектральна істота чинить опір лише фізичним ударам.
    int apply_defense(int amount, DamageType type) const override {
        if (type == DamageType::Magical) {
            return Character::apply_defense(amount, type);
        }

        int reduced_amount = static_cast<int>(amount * physical_resistance_);
        if (reduced_amount < 1 && amount > 0) {
            reduced_amount = 1;
        }

        return Character::apply_defense(reduced_amount, type);
    }
};

#endif // WRAITH_HPP
