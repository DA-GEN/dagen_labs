#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <iostream>

/**
 * @brief Abstract base class for all characters in the game
 */
class Character {
protected:
    std::string name_;      ///< Character name
    int hp_;                ///< Current health points
    int max_hp_;            ///< Maximum health points
    int attack_power_;      ///< Attack damage value
    int defense_;           ///< Defense value that reduces incoming damage

public:
    /**
     * @brief Construct a new Character
     * @param name Character name
     * @param max_hp Maximum health points
     * @param attack_power Attack damage value
     * @param defense Defense value
     */
    Character(const std::string& name, int max_hp, int attack_power, int defense)
        : name_(name), hp_(max_hp), max_hp_(max_hp), 
          attack_power_(attack_power), defense_(defense) {}

    virtual ~Character() = default;

    /**
     * @brief Attack another character (pure virtual)
     * @param target Target character to attack
     */
    virtual void attack(Character& target) = 0;

    /**
     * @brief Take damage from an attack
     * @param amount Raw damage amount before defense calculation
     */
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

    /**
     * @brief Check if character is alive
     * @return true if HP > 0, false otherwise
     */
    bool is_alive() const {
        return hp_ > 0;
    }

    /**
     * @brief Heal the character
     * @param amount Amount of HP to restore
     */
    void heal(int amount) {
        if (amount <= 0) return;
        
        hp_ += amount;
        if (hp_ > max_hp_) {
            hp_ = max_hp_;
        }

        std::cout << name_ << " відновлює " << amount << " HP. ";
        std::cout << "(HP: " << hp_ << "/" << max_hp_ << ")" << std::endl;
    }

    /**
     * @brief Display character statistics
     */
    virtual void display_stats() const {
        std::cout << "\n=== " << name_ << " ===" << std::endl;
        std::cout << "Здоров'я: " << hp_ << "/" << max_hp_ << std::endl;
        std::cout << "Атака: " << attack_power_ << std::endl;
        std::cout << "Захист: " << defense_ << std::endl;
    }

    /** @brief Get character name */
    std::string get_name() const { return name_; }
    
    /** @brief Get current HP */
    int get_hp() const { return hp_; }
    
    /** @brief Get maximum HP */
    int get_max_hp() const { return max_hp_; }
    
    /** @brief Get attack power */
    int get_attack_power() const { return attack_power_; }
    
    /** @brief Get defense value */
    int get_defense() const { return defense_; }

    /**
     * @brief Set HP directly (used by Item system)
     * @param hp New HP value
     */
    void set_hp(int hp) { 
        hp_ = hp;
        if (hp_ > max_hp_) hp_ = max_hp_;
        if (hp_ < 0) hp_ = 0;
    }

    /**
     * @brief Modify attack power
     * @param delta Amount to add to attack power
     */
    void modify_attack_power(int delta) { attack_power_ += delta; }
    
    /**
     * @brief Modify defense
     * @param delta Amount to add to defense
     */
    void modify_defense(int delta) { defense_ += delta; }
};

#endif // CHARACTER_HPP
