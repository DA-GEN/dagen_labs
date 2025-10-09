#ifndef MAGE_HPP
#define MAGE_HPP

#include "Player.hpp"

class Mage : public Player {
public:
    Mage(const std::string& name)
        : Player(name, 80, 30, 5) {}

    void attack(Character& target) override {
        std::cout << name_ << " (Маг) накладає МАГІЧНЕ ЗАКЛЯТТЯ на " 
                  << target.get_name() << "!" << std::endl;
        
        // Magic bypasses take_damage() to ignore defense completely
        int damage = attack_power_;
        int old_hp = target.get_hp();
        target.set_hp(old_hp - damage);
        
        std::cout << target.get_name() << " отримує " << damage 
                  << " магічних пошкоджень (ІГНОРУЄ ЗАХИСТ)! ";
        std::cout << "(HP: " << target.get_hp() << "/" 
                  << target.get_max_hp() << ")" << std::endl;
    }

    void display_stats() const override {
        Player::display_stats();
        std::cout << "Клас: Маг" << std::endl;
        std::cout << "Особливість: Атаки ігнорують захист ворога" << std::endl;
    }
};

#endif // MAGE_HPP

