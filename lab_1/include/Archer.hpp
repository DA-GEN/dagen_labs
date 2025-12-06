#ifndef ARCHER_HPP
#define ARCHER_HPP

#include "Player.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

class Archer : public Player {
private:
    static bool random_initialized_;
    const int crit_chance_ = 30;

public:
    Archer(const std::string& name)
        : Player(name, 100, 20, 8) {

        if (!random_initialized_) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            random_initialized_ = true;
        }
    }

    std::string attack(Character& target) override {
        int roll = std::rand() % 100;
        bool is_crit = (roll < crit_chance_);

        int damage = attack_power_;
        std::string prefix = "🏹 " + name_ + " стріляє з лука. ";

        if (is_crit) {
            damage *= 2;
            prefix = "🏹🎯 " + name_ + " завдає КРИТИЧНОГО УДАРУ! ";
        }

        std::string damage_log = target.take_damage(damage);
        return prefix + damage_log;
    }

    std::string get_stats_string() const override {
        return Player::get_stats_string() + " [Клас: Лучник (Шанс крита 30%)]";
    }
};

bool Archer::random_initialized_ = false;

#endif // ARCHER_HPP