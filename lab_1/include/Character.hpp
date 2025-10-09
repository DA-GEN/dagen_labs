#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <iostream>

class Character {
protected:
    std::string name_;
    int hp_;
    int max_hp_;
    int attack_power_;
    int defense_;

public:
    Character(const std::string& name, int max_hp, int attack_power, int defense)
        : name_(name), hp_(max_hp), max_hp_(max_hp), 
          attack_power_(attack_power), defense_(defense) {}

    virtual ~Character() = default;

    virtual void attack(Character& target) = 0;

    virtual void take_damage(int amount) {
        if (amount <= 0) return;

        // Defense reduces damage, but at least 1 damage always gets through
        int actual_damage = amount - defense_;
        if (actual_damage < 1 && amount > 0) {
            actual_damage = 1;
        }

        hp_ -= actual_damage;
        if (hp_ < 0) {
            hp_ = 0;
        }

        std::cout << name_ << " отримує " << actual_damage << " пошкоджень! ";
        std::cout << "(HP: " << hp_ << "/" << max_hp_ << ")" << std::endl;
    }

    bool is_alive() const {
        return hp_ > 0;
    }

    void heal(int amount) {
        if (amount <= 0) return;
        
        hp_ += amount;
        if (hp_ > max_hp_) {
            hp_ = max_hp_;
        }

        std::cout << name_ << " відновлює " << amount << " HP. ";
        std::cout << "(HP: " << hp_ << "/" << max_hp_ << ")" << std::endl;
    }

    virtual void display_stats() const {
        std::cout << "\n=== " << name_ << " ===" << std::endl;
        std::cout << "Здоров'я: " << hp_ << "/" << max_hp_ << std::endl;
        std::cout << "Атака: " << attack_power_ << std::endl;
        std::cout << "Захист: " << defense_ << std::endl;
    }

    std::string get_name() const { return name_; }
    int get_hp() const { return hp_; }
    int get_max_hp() const { return max_hp_; }
    int get_attack_power() const { return attack_power_; }
    int get_defense() const { return defense_; }

    // Used by Item system to apply equipment bonuses
    void set_hp(int hp) { 
        hp_ = hp;
        if (hp_ > max_hp_) hp_ = max_hp_;
        if (hp_ < 0) hp_ = 0;
    }

    void modify_attack_power(int delta) { attack_power_ += delta; }
    void modify_defense(int delta) { defense_ += delta; }
};

#endif // CHARACTER_HPP
