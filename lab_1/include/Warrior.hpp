#ifndef WARRIOR_HPP
#define WARRIOR_HPP

#include "Player.hpp"

class Warrior : public Player {
public:
    Warrior(const std::string& name)
        : Player(name, 150, 25, 12) {}

    void attack(Character& target) override {
        std::cout << name_ << " (Воїн) завдає ПОТУЖНОГО УДАРУ по " 
                  << target.get_name() << "!" << std::endl;
        
        int damage = static_cast<int>(attack_power_ * 1.2);  // 120% damage modifier
        target.take_damage(damage);
    }

    void display_stats() const override {
        Player::display_stats();
        std::cout << "Клас: Воїн" << std::endl;
        std::cout << "Особливість: 120% пошкоджень при атаці" << std::endl;
    }
};

#endif // WARRIOR_HPP

