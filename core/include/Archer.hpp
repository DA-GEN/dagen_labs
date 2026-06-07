#ifndef ARCHER_HPP
#define ARCHER_HPP

#include "Player.hpp"
#include "RandomProvider.hpp"
#include <string>

class Archer : public Player {
private:
    // ЗМІНА: замість власної статичної ініціалізації std::rand (як було
    // раніше і дублювалося в GameMap) лучник отримує спільний генератор
    // випадкових чисел через ін'єкцію залежності (Dependency Injection).
    // Це усуває дублювання й дозволяє детерміновано тестувати шанс криту.
    IRandomProvider& rng_;
    static constexpr int crit_chance_ = 30;

public:
    Archer(const std::string& name, IRandomProvider& rng)
        : Player(name, 100, 20, 8), rng_(rng) {
    }

    std::string attack(Character& target) override {
        bool is_crit = rng_.roll_chance(crit_chance_);

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

#endif // ARCHER_HPP
