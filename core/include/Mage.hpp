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
        // ЗМІНА: магія більше не обходить інкапсуляцію цілі через set_hp().
        // Замість цього шкода передається через звичайний поліморфний
        // take_damage з типом DamageType::Magical - ціль сама вирішує,
        // як інтерпретувати магічну шкоду (наприклад, Wraith пропускає її
        // крізь свій фізичний резист, а базовий Character ігнорує захист).
        int damage = attack_power_;
        std::string damage_log = target.take_damage(damage, DamageType::Magical);

        return "🔥 " + name_ + " кастує закляття! " + damage_log + " (ІГНОР ФІЗИЧНОГО ЗАХИСТУ)";
    }

    std::string get_stats_string() const override {
        return Player::get_stats_string() + " [Клас: Маг (Ігнор захисту)]";
    }
};

#endif // MAGE_HPP
