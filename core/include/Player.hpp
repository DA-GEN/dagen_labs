#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Item.hpp"
#include <vector>
#include <string>
#include <memory>
#include <sstream>

class Player : public Character {
protected:
    std::vector<Item*> inventory_;

public:
    Player(const std::string& name, int max_hp, int attack_power, int defense)
        : Character(name, max_hp, attack_power, defense) {
    }

    virtual ~Player() = default;

    // Просто додає предмет, повідомлення генерує Game
    void add_item(Item* item) {
        if (item != nullptr) {
            inventory_.push_back(item);
        }
    }

    // Повертає список рядків для відображення інвентарю в GUI
    std::vector<std::string> get_inventory_list() const {
        std::vector<std::string> list;
        if (inventory_.empty()) {
            list.push_back("Інвентар порожній");
        }
        else {
            for (size_t i = 0; i < inventory_.size(); ++i) {
                list.push_back(std::to_string(i + 1) + ". " + inventory_[i]->get_name());
            }
        }
        return list;
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

    // Для відображення повної статистики в GUI
    std::string get_full_stats() const {
        std::ostringstream ss;
        ss << get_stats_string() << "\nПредметів в сумці: " << inventory_.size();
        return ss.str();
    }

    // Перевизначення display_stats не потрібне, якщо ми не використовуємо консоль,
    // але для сумісності з Character залишаємо базову реалізацію.
    // Атака має бути реалізована в класах Warrior/Mage/Archer
};

#endif // PLAYER_HPP