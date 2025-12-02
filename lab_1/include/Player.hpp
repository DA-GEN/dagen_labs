#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Item.hpp"
#include <vector>
#include <memory>

/**
 * @brief Base class for player characters with inventory management
 */
class Player : public Character {
protected:
    std::vector<Item*> inventory_;  ///< Player inventory

public:
    /**
     * @brief Construct a new Player
     * @param name Player name
     * @param max_hp Maximum health points
     * @param attack_power Attack damage value
     * @param defense Defense value
     */
    Player(const std::string& name, int max_hp, int attack_power, int defense)
        : Character(name, max_hp, attack_power, defense) {}

    virtual ~Player() = default;

    /**
     * @brief Add item to inventory
     * @param item Pointer to item to add
     */
    void add_item(Item* item) {
        if (item != nullptr) {
            inventory_.push_back(item);
            std::cout << name_ << " підбирає " << item->get_name() << std::endl;
        }
    }

    /**
     * @brief Display inventory contents
     */
    void show_inventory() const {
        std::cout << "\n=== Інвентар: " << name_ << " ===" << std::endl;
        if (inventory_.empty()) {
            std::cout << "Порожньо" << std::endl;
        } else {
            for (size_t i = 0; i < inventory_.size(); ++i) {
                std::cout << (i + 1) << ". " << inventory_[i]->get_name() 
                         << " - " << inventory_[i]->get_description() << std::endl;
            }
        }
    }

    /**
     * @brief Get item at index
     * @param index Inventory index
     * @return Pointer to item or nullptr if invalid index
     */
    Item* get_item(size_t index) {
        if (index < inventory_.size()) {
            return inventory_[index];
        }
        return nullptr;
    }

    /**
     * @brief Remove item from inventory
     * @param index Inventory index
     */
    void remove_item(size_t index) {
        if (index < inventory_.size()) {
            inventory_.erase(inventory_.begin() + index);
        }
    }

    /**
     * @brief Get inventory size
     * @return Number of items in inventory
     */
    size_t inventory_size() const {
        return inventory_.size();
    }

    void display_stats() const override {
        Character::display_stats();
        std::cout << "Інвентар: " << inventory_.size() << " предметів" << std::endl;
    }
};

#endif // PLAYER_HPP

