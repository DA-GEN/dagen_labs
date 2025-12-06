#ifndef MAGE_HPP
#define MAGE_HPP

#include "Player.hpp"
#include <string>

class Mage : public Player {
public:
    Mage(const std::string& name)
        : Player(name, 80, 30, 5) {
    }

    std::string attack(Character& target) override {
        // Магія ігнорує захист
        int damage = attack_power_;
        int old_hp = target.get_hp();
        target.set_hp(old_hp - damage);

        return "🔥 " + name_ + " кастує закляття! " + target.get_name() +
            " отримує " + std::to_string(damage) + " шкоди (ІГНОР ЗАХИСТУ).";
    }

    std::string get_stats_string() const override {
        return Player::get_stats_string() + " [Клас: Маг (Ігнор захисту)]";
    }
};

#endif // MAGE_HPP