#ifndef WARRIOR_HPP
#define WARRIOR_HPP

#include "Player.hpp"
#include <string>

class Warrior : public Player {
public:
    Warrior(const std::string& name)
        : Player(name, 150, 25, 12) {
    }

    std::string attack(Character& target) override {
        int damage = static_cast<int>(attack_power_ * 1.2);  // 120% пошкоджень

        // Викликаємо take_damage у цілі і отримуємо результат
        std::string damage_log = target.take_damage(damage);

        return "⚔️ " + name_ + " (Воїн) завдає ПОТУЖНОГО УДАРУ! " + damage_log;
    }

    std::string get_stats_string() const override {
        return Player::get_stats_string() + " [Клас: Воїн (+20% атаки)]";
    }
};

#endif // WARRIOR_HPP