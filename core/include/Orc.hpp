#ifndef ORC_HPP
#define ORC_HPP

#include "Enemy.hpp"
#include <string>

class Orc : public Enemy {
public:
    Orc(const std::string& name = "Orc")
        : Enemy(name, 100, 20, 10) {
    }

    std::string attack(Character& target) override {
        int damage = static_cast<int>(attack_power_ * 1.1); // 110% damage
        std::string damage_log = target.take_damage(damage);

        return "👹 " + name_ + " (Орк) завдає БРУТАЛЬНОГО УДАРУ! " + damage_log;
    }

    std::string get_stats_string() const override {
        return Character::get_stats_string() + " (Орк: +10% пошкоджень)";
    }
};

#endif // ORC_HPP