#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

class Enemy : public Character {
public:
    Enemy(const std::string& name, int max_hp, int attack_power, int defense)
        : Character(name, max_hp, attack_power, defense) {}

    virtual ~Enemy() = default;

    void display_stats() const override {
        Character::display_stats();
        std::cout << "Тип: Ворог" << std::endl;
    }
};

#endif // ENEMY_HPP

