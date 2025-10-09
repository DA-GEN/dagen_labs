#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include "Enemy.hpp"

class Goblin : public Enemy {
public:
    Goblin(const std::string& name = "Goblin")
        : Enemy(name, 40, 10, 3) {}

    void attack(Character& target) override {
        std::cout << name_ << " (Гоблін) завдає швидкого удару по " 
                  << target.get_name() << "!" << std::endl;
        
        target.take_damage(attack_power_);
    }

    void display_stats() const override {
        Character::display_stats();
        std::cout << "Тип: Гоблін (Слабкий ворог)" << std::endl;
    }
};

#endif // GOBLIN_HPP

