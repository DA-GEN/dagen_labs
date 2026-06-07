#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include "Enemy.hpp"
#include <string>

class Goblin : public Enemy {
public:
    Goblin(const std::string& name = "Goblin")
        : Enemy(name, 40, 10, 3) {
    }

    std::string attack(Character& target) override {
        std::string damage_log = target.take_damage(attack_power_);
        return "👺 " + name_ + " (Гоблін) швидко атакує! " + damage_log;
    }

    std::string get_stats_string() const override {
        return Character::get_stats_string() + " (Гоблін: Слабкий ворог)";
    }
};

#endif // GOBLIN_HPP