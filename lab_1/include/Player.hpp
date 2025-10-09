#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Item.hpp"
#include <vector>
#include <memory>

class Player : public Character {
protected:
    std::vector<Item*> inventory_;

public:
    Player(const std::string& name, int max_hp, int attack_power, int defense)
        : Character(name, max_hp, attack_power, defense) {}

    virtual ~Player() = default;

    void add_item(Item* item) {
        if (item != nullptr) {
            inventory_.push_back(item);
            std::cout << name_ << " підбирає " << item->get_name() << std::endl;
        }
    }

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

    Item* get_item(size_t index) {
        if (index < inventory_.size()) {
            return inventory_[index];
        }
        return nullptr;
    }

    void remove_item(size_t index) {
        if (index < inventory_.size()) {
            inventory_.erase(inventory_.begin() + index);
        }
    }

    size_t inventory_size() const {
        return inventory_.size();
    }

    void display_stats() const override {
        Character::display_stats();
        std::cout << "Інвентар: " << inventory_.size() << " предметів" << std::endl;
    }
};

#endif // PLAYER_HPP

