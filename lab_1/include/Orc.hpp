#ifndef ORC_HPP
#define ORC_HPP

#include "Enemy.hpp"

/**
 * @brief Orc enemy - strong enemy with 110% damage modifier
 */
class Orc : public Enemy {
public:
    /**
     * @brief Construct a new Orc
     * @param name Orc name (default: "Orc")
     */
    Orc(const std::string& name = "Orc")
        : Enemy(name, 100, 20, 10) {}

    void attack(Character& target) override {
        std::cout << name_ << " (Орк) наносить БРУТАЛЬНИЙ УДАР по " 
                  << target.get_name() << "!" << std::endl;
        
        int damage = static_cast<int>(attack_power_ * 1.1);  // 110% damage modifier
        target.take_damage(damage);
    }

    void display_stats() const override {
        Character::display_stats();
        std::cout << "Тип: Орк (Сильний ворог)" << std::endl;
        std::cout << "Особливість: 110% пошкоджень при атаці" << std::endl;
    }
};

#endif // ORC_HPP

