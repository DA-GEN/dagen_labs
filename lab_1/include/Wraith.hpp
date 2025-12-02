#ifndef WRAITH_HPP
#define WRAITH_HPP

#include "Enemy.hpp"

/**
 * @brief Wraith enemy - spectral enemy with 50% physical resistance and life drain
 */
class Wraith : public Enemy {
private:
    const double physical_resistance_ = 0.5;  ///< Physical damage resistance (50%)

public:
    /**
     * @brief Construct a new Wraith
     * @param name Wraith name (default: "Wraith")
     */
    Wraith(const std::string& name = "Wraith")
        : Enemy(name, 60, 18, 5) {}

    void attack(Character& target) override {
        std::cout << name_ << " (Примара) вивільняє СПЕКТРАЛЬНЕ ВИСМОКТУВАННЯ на " 
                  << target.get_name() << "!" << std::endl;
        
        target.take_damage(attack_power_);
        
        int heal_amount = attack_power_ / 3;  // Life drain: heals for 30% of damage
        heal(heal_amount);
    }

    /**
     * @brief Take damage with physical resistance applied
     * @param amount Raw damage amount
     */
    void take_damage(int amount) override {
        if (amount <= 0) return;

        int reduced_amount = static_cast<int>(amount * physical_resistance_);
        if (reduced_amount < 1 && amount > 0) {
            reduced_amount = 1;
        }

        std::cout << name_ << " (Примара) проходить крізь атаку! ";
        std::cout << "(Фізичний опір: " 
                  << static_cast<int>(physical_resistance_ * 100) << "%)" << std::endl;

        int actual_damage = reduced_amount - defense_;
        if (actual_damage < 1 && reduced_amount > 0) {
            actual_damage = 1;
        }

        hp_ -= actual_damage;
        if (hp_ < 0) {
            hp_ = 0;
        }

        std::cout << name_ << " отримує " << actual_damage << " пошкоджень! ";
        std::cout << "(HP: " << hp_ << "/" << max_hp_ << ")" << std::endl;
    }

    void display_stats() const override {
        Character::display_stats();
        std::cout << "Тип: Примара (Спектральний ворог)" << std::endl;
        std::cout << "Особливість: Отримує на 50% менше фізичних пошкоджень, висмоктує HP при атаці" << std::endl;
    }
};

#endif // WRAITH_HPP

