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

        return "👻 " + name_ + " (Примара) використовує СПЕКТРАЛЬНЕ ВИСМОКТУВАННЯ! " +
            damage_log + " (Примара відновила сили)";
    }

    // Перевизначення take_damage для обробки резистів
    std::string take_damage(int amount) override {
        if (amount <= 0) return name_ + " не отримує пошкоджень.";

        int reduced_amount = static_cast<int>(amount * physical_resistance_);
        if (reduced_amount < 1 && amount > 0) {
            reduced_amount = 1;
        }

        // Розрахунок захисту (логіка дублюється з Character, щоб врахувати резист)
        int actual_damage = reduced_amount - defense_;
        if (actual_damage < 1 && reduced_amount > 0) {
            actual_damage = 1;
        }

        hp_ -= actual_damage;
        if (hp_ < 0) hp_ = 0;

        std::ostringstream ss;
        ss << "👻 " << name_ << " проходить крізь атаку (50% резист)! Отримує лише "
            << actual_damage << " шкоди. (HP: " << hp_ << "/" << max_hp_ << ")";

        return ss.str();
    }

    std::string get_stats_string() const override {
        return Character::get_stats_string() + " [Примара: 50% фіз. резист, вампіризм]";
    }
};

#endif // WRAITH_HPP