#ifndef ITEM_FACTORY_HPP
#define ITEM_FACTORY_HPP

#include "Item.hpp"
#include "Weapon.hpp"
#include "Armor.hpp"
#include "Potion.hpp"
#include "RandomProvider.hpp"
#include <memory>
#include <string>
#include <vector>

// Factory Method для предметів - аналогічно до IEnemyFactory виносить
// з GameMap знання про конкретні класи Weapon/Armor/Potion та діапазони
// їхніх характеристик (OCP: новий тип предмета додається без правок GameMap).
class IItemFactory {
public:
    virtual ~IItemFactory() = default;
    virtual std::unique_ptr<Item> create_random() const = 0;
};

class ItemFactory : public IItemFactory {
public:
    explicit ItemFactory(IRandomProvider& rng) : rng_(rng) {}

    std::unique_ptr<Item> create_random() const override {
        switch (rng_.next_int(0, 2)) {
        case 0: return create_weapon();
        case 1: return create_armor();
        default: return create_potion();
        }
    }

private:
    IRandomProvider& rng_;

    static const std::vector<std::string>& weapon_names() {
        static const std::vector<std::string> names = {
            "Іржавий меч", "Залізна сокира", "Стальний кинджал", "Стародавня булава", "Ельфійський лук"
        };
        return names;
    }

    static const std::vector<std::string>& armor_names() {
        static const std::vector<std::string> names = {
            "Шкіряний жилет", "Кольчуга", "Залізний щит", "Латний обладунок", "Магічний плащ"
        };
        return names;
    }

    static const std::vector<std::string>& potion_names() {
        static const std::vector<std::string> names = {
            "Зілля здоров'я", "Еліксир", "Цілющий настій", "Фляга відновлення", "Есенція життя"
        };
        return names;
    }

    std::unique_ptr<Item> create_weapon() const {
        const auto& names = weapon_names();
        const std::string& chosen_name = names[static_cast<size_t>(rng_.next_int(0, static_cast<int>(names.size()) - 1))];
        int damage = rng_.next_int(15, 39);
        return std::make_unique<Weapon>(chosen_name, "Надійна зброя", damage);
    }

    std::unique_ptr<Item> create_armor() const {
        const auto& names = armor_names();
        const std::string& chosen_name = names[static_cast<size_t>(rng_.next_int(0, static_cast<int>(names.size()) - 1))];
        int defense = rng_.next_int(10, 29);
        return std::make_unique<Armor>(chosen_name, "Захисне спорядження", defense);
    }

    std::unique_ptr<Item> create_potion() const {
        const auto& names = potion_names();
        const std::string& chosen_name = names[static_cast<size_t>(rng_.next_int(0, static_cast<int>(names.size()) - 1))];
        int heal = rng_.next_int(20, 59);
        return std::make_unique<Potion>(chosen_name, "Відновлює здоров'я", heal);
    }
};

#endif // ITEM_FACTORY_HPP
