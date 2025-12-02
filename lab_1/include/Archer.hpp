#ifndef ARCHER_HPP
#define ARCHER_HPP

#include "Player.hpp"
#include <cstdlib>
#include <ctime>

/**
 * @brief Archer class with 30% critical hit chance (2x damage)
 */
class Archer : public Player {
private:
    static bool random_initialized_;  ///< Random number generator initialization flag
    const int crit_chance_ = 30;      ///< Critical hit chance percentage

public:
    /**
     * @brief Construct a new Archer
     * @param name Archer name
     */
    Archer(const std::string& name)
        : Player(name, 100, 20, 8) {
        
        if (!random_initialized_) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            random_initialized_ = true;
        }
    }

    void attack(Character& target) override {
        int roll = std::rand() % 100;
        bool is_crit = (roll < crit_chance_);

        if (is_crit) {
            std::cout << name_ << " (Лучник) випускає КРИТИЧНУ СТРІЛУ в " 
                      << target.get_name() << "! ***КРИТИЧНИЙ УДАР***" << std::endl;
            
            int damage = attack_power_ * 2;
            target.take_damage(damage);
        } else {
            std::cout << name_ << " (Лучник) стріляє стрілою в " 
                      << target.get_name() << std::endl;
            
            target.take_damage(attack_power_);
        }
    }

    void display_stats() const override {
        Player::display_stats();
        std::cout << "Клас: Лучник" << std::endl;
        std::cout << "Особливість: " << crit_chance_ 
                  << "% шанс критичного удару (2x пошкодження)" << std::endl;
    }
};

bool Archer::random_initialized_ = false;

#endif // ARCHER_HPP

