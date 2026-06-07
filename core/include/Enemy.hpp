#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"
#include <string>

class Enemy : public Character {
public:
    Enemy(const std::string& name, int max_hp, int attack_power, int defense)
        : Character(name, max_hp, attack_power, defense) {
    }

    virtual ~Enemy() = default;

    // ЗМІНА: Перевизначений get_stats_string замість display_stats
    std::string get_stats_string() const override {
        return Character::get_stats_string() + " | Тип: Ворог";
    }
};

#endif // ENEMY_HPP