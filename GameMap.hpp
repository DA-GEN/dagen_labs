#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "Graph.hpp"
#include "MapNode.hpp"
#include "Enemy.hpp"
#include "Goblin.hpp"
#include "Orc.hpp"
#include "Wraith.hpp"
#include "Item.hpp"
#include "Weapon.hpp"
#include "Armor.hpp"
#include "Potion.hpp"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <sstream>
#include <string>

class GameMap {
private:
    Graph<MapNode*> graph_;
    std::vector<std::unique_ptr<MapNode>> nodes_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Item>> items_;

    static bool random_initialized_;

    void init_random() {
        if (!random_initialized_) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            random_initialized_ = true;
        }
    }

    std::string generate_room_description(int /* id */) {
        const std::vector<std::string> room_types = {
            "Темний коридор", "Стародавня зала", "Затхле підземелля",
            "Кам'яний прохід", "Освітлений факелами прохід", "Занедбаний склеп",
            "Таємнича скарбниця", "Тінява ніша", "Зруйнована крипта", "Підземна печера"
        };

        const std::vector<std::string> features = {
            "вкрита павутинням", "з якої крапає вода", "зі смородом гнилі",
            "що відлунює шепотами", "вкрита мохом", "сповнена туману",
            "обкладена кістками", "вирізьблена рунами", "тьмяно освітлена", "моторошно тиха"
        };

        int type_idx = std::rand() % room_types.size();
        int feature_idx = std::rand() % features.size();

        return room_types[type_idx] + " " + features[feature_idx];
    }

    std::unique_ptr<Enemy> create_random_enemy() {
        int type = std::rand() % 3;
        switch (type) {
        case 0: return std::make_unique<Goblin>("Гоблін");
        case 1: return std::make_unique<Orc>("Орк");
        case 2: return std::make_unique<Wraith>("Примара");
        default: return std::make_unique<Goblin>("Гоблін");
        }
    }

    std::unique_ptr<Item> create_random_item() {
        int type = std::rand() % 3;

        const std::vector<std::string> weapon_names = { "Іржавий меч", "Залізна сокира", "Стальний кинджал", "Стародавня булава", "Ельфійський лук" };
        const std::vector<std::string> armor_names = { "Шкіряний жилет", "Кольчуга", "Залізний щит", "Латний обладунок", "Магічний плащ" };
        const std::vector<std::string> potion_names = { "Зілля здоров'я", "Еліксир", "Цілющий настій", "Фляга відновлення", "Есенція життя" };

        switch (type) {
        case 0: {
            int idx = std::rand() % weapon_names.size();
            int damage = 15 + std::rand() % 25;
            return std::make_unique<Weapon>(weapon_names[idx], "Надійна зброя", damage);
        }
        case 1: {
            int idx = std::rand() % armor_names.size();
            int defense = 10 + std::rand() % 20;
            return std::make_unique<Armor>(armor_names[idx], "Захисне спорядження", defense);
        }
        case 2: {
            int idx = std::rand() % potion_names.size();
            int heal = 20 + std::rand() % 40;
            return std::make_unique<Potion>(potion_names[idx], "Відновлює здоров'я", heal);
        }
        default: return std::make_unique<Potion>("Зілля здоров'я", "Відновлює здоров'я", 30);
        }
    }

public:
    GameMap() { init_random(); }
    ~GameMap() = default;

    void generate_map(int num_rooms, int num_enemies, int num_items) {
        nodes_.clear();
        enemies_.clear();
        items_.clear();
        // Graph очищується автоматично при перестворенні, або треба додати метод clear() у Graph.hpp
        // Припустимо, що Graph очищується сам або ми створюємо новий об'єкт

        for (int i = 0; i < num_rooms; ++i) {
            std::string desc = generate_room_description(i);
            auto node = std::make_unique<MapNode>(i, desc);
            graph_.add_node(node.get());
            nodes_.push_back(std::move(node));
        }

        // Лінійний шлях
        for (int i = 0; i < num_rooms - 1; ++i) {
            graph_.add_undirected_edge(nodes_[i].get(), nodes_[i + 1].get());
        }

        // Випадкові з'єднання
        int extra_connections = num_rooms / 2;
        for (int i = 0; i < extra_connections; ++i) {
            int from = std::rand() % num_rooms;
            int to = std::rand() % num_rooms;
            if (from != to && !graph_.has_edge(nodes_[from].get(), nodes_[to].get())) {
                graph_.add_undirected_edge(nodes_[from].get(), nodes_[to].get());
            }
        }

        // Розміщення ворогів і предметів
        std::vector<int> available_rooms(num_rooms);
        for (int i = 0; i < num_rooms; ++i) available_rooms[i] = i;

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(available_rooms.begin(), available_rooms.end(), g);

        for (int i = 0; i < num_enemies && i < num_rooms; ++i) {
            auto enemy = create_random_enemy();
            nodes_[available_rooms[i]]->set_enemy(enemy.get());
            enemies_.push_back(std::move(enemy));
        }

        std::shuffle(available_rooms.begin(), available_rooms.end(), g);

        for (int i = 0; i < num_items && i < num_rooms; ++i) {
            auto item = create_random_item();
            nodes_[available_rooms[i]]->set_item(item.get());
            items_.push_back(std::move(item));
        }
    }

    MapNode* get_node_by_id(int id) {
        if (id >= 0 && id < static_cast<int>(nodes_.size())) {
            return nodes_[id].get();
        }
        return nullptr;
    }

    std::vector<MapNode*> get_neighbors(int id) {
        MapNode* node = get_node_by_id(id);
        if (!node) return {};
        return graph_.get_neighbors(node);
    }

    size_t get_num_rooms() const {
        return nodes_.size();
    }

    // get_path і інші методи можна залишити, якщо вони не використовують cout
};

inline bool GameMap::random_initialized_ = false;

#endif // GAMEMAP_HPP
