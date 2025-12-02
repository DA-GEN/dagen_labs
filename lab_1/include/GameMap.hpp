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

/**
 * @brief Manages the dungeon map with procedural generation
 */
class GameMap {
private:
    Graph<MapNode*> graph_;                          ///< Graph structure for room connections
    std::vector<std::unique_ptr<MapNode>> nodes_;    ///< All map nodes
    std::vector<std::unique_ptr<Enemy>> enemies_;    ///< All enemies
    std::vector<std::unique_ptr<Item>> items_;       ///< All items
    
    static bool random_initialized_;  ///< Random initialization flag

    /** @brief Initialize random number generator */
    void init_random() {
        if (!random_initialized_) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            random_initialized_ = true;
        }
    }

    /**
     * @brief Generate random room description
     * @return Room description string
     */
    std::string generate_room_description(int /* id */) {
        const std::vector<std::string> room_types = {
            "Темний коридор",
            "Стародавня зала",
            "Затхле підземелля",
            "Кам'яний прохід",
            "Освітлений факелами прохід",
            "Занедбаний склеп",
            "Таємнича скарбниця",
            "Тінява ніша",
            "Зруйнована крипта",
            "Підземна печера"
        };
        
        const std::vector<std::string> features = {
            "вкрита павутинням",
            "з якої крапає вода",
            "зі смородом гнилі",
            "що відлунює шепотами",
            "вкрита мохом",
            "сповнена туману",
            "обкладена кістками",
            "вирізьблена рунами",
            "тьмяно освітлена",
            "моторошно тиха"
        };
        
        int type_idx = std::rand() % room_types.size();
        int feature_idx = std::rand() % features.size();
        
        std::ostringstream desc;
        desc << room_types[type_idx] << " " << features[feature_idx];
        
        return desc.str();
    }

    /**
     * @brief Create random enemy
     * @return Unique pointer to enemy
     */
    std::unique_ptr<Enemy> create_random_enemy() {
        int type = std::rand() % 3;
        
        switch (type) {
            case 0:
                return std::make_unique<Goblin>("Гоблін");
            case 1:
                return std::make_unique<Orc>("Орк");
            case 2:
                return std::make_unique<Wraith>("Примара");
            default:
                return std::make_unique<Goblin>("Гоблін");
        }
    }

    /**
     * @brief Create random item
     * @return Unique pointer to item
     */
    std::unique_ptr<Item> create_random_item() {
        int type = std::rand() % 3;
        
        const std::vector<std::string> weapon_names = {
            "Іржавий меч", "Залізна сокира", "Стальний кинджал", "Стародавня булава", "Ельфійський лук"
        };
        const std::vector<std::string> armor_names = {
            "Шкіряний жилет", "Кольчуга", "Залізний щит", "Латний обладунок", "Магічний плащ"
        };
        const std::vector<std::string> potion_names = {
            "Зілля здоров'я", "Еліксир", "Цілющий настій", "Фляга відновлення", "Есенція життя"
        };
        
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
            default:
                return std::make_unique<Potion>("Зілля здоров'я", "Відновлює здоров'я", 30);
        }
    }

public:
    /**
     * @brief Construct a new GameMap
     */
    GameMap() {
        init_random();
    }

    ~GameMap() = default;

    /**
     * @brief Generate procedural dungeon map
     * @param num_rooms Number of rooms to generate
     * @param num_enemies Number of enemies to place
     * @param num_items Number of items to place
     */
    void generate_map(int num_rooms, int num_enemies, int num_items) {
        nodes_.clear();
        enemies_.clear();
        items_.clear();

        for (int i = 0; i < num_rooms; ++i) {
            std::string desc = generate_room_description(i);
            auto node = std::make_unique<MapNode>(i, desc);
            graph_.add_node(node.get());
            nodes_.push_back(std::move(node));
        }

        // Guarantee connectivity: linear path ensures all rooms reachable
        for (int i = 0; i < num_rooms - 1; ++i) {
            graph_.add_undirected_edge(nodes_[i].get(), nodes_[i + 1].get());
        }

        // Add extra connections for more interesting map layout
        int extra_connections = num_rooms / 2;
        for (int i = 0; i < extra_connections; ++i) {
            int from = std::rand() % num_rooms;
            int to = std::rand() % num_rooms;
            
            if (from != to && !graph_.has_edge(nodes_[from].get(), nodes_[to].get())) {
                graph_.add_undirected_edge(nodes_[from].get(), nodes_[to].get());
            }
        }

        // Shuffle to randomize enemy/item placement
        std::vector<int> available_rooms;
        for (int i = 0; i < num_rooms; ++i) {
            available_rooms.push_back(i);
        }
        
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

    /**
     * @brief Print entire map to console
     */
    void print_map() const {
        std::cout << "\n╔════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║           КАРТА ПІДЗЕМЕЛЛЯ                         ║" << std::endl;
        std::cout << "╚════════════════════════════════════════════════════╝\n" << std::endl;
        
        std::cout << "Всього кімнат: " << nodes_.size() << std::endl;
        std::cout << "Всього ворогів: " << enemies_.size() << std::endl;
        std::cout << "Всього предметів: " << items_.size() << "\n" << std::endl;

        for (const auto& node : nodes_) {
            node->display();
            
            auto neighbors = graph_.get_neighbors(node.get());
            if (!neighbors.empty()) {
                std::cout << "  Виходи до: ";
                for (size_t i = 0; i < neighbors.size(); ++i) {
                    std::cout << "Кімната " << neighbors[i]->get_id();
                    if (i < neighbors.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << std::endl;
            } else {
                std::cout << "  (Немає виходів)" << std::endl;
            }
            
            std::cout << std::endl;
        }
    }

    /**
     * @brief Get node by ID
     * @param id Room ID
     * @return Pointer to MapNode or nullptr
     */
    MapNode* get_node_by_id(int id) {
        if (id >= 0 && id < static_cast<int>(nodes_.size())) {
            return nodes_[id].get();
        }
        return nullptr;
    }

    /**
     * @brief Find path between two rooms
     * @param start_id Start room ID
     * @param end_id End room ID
     * @return Path as vector of MapNode pointers
     */
    std::vector<MapNode*> find_path(int start_id, int end_id) {
        MapNode* start = get_node_by_id(start_id);
        MapNode* end = get_node_by_id(end_id);
        
        if (!start || !end) {
            return {};
        }
        
        return graph_.bfs(start, end);
    }

    /**
     * @brief Get neighboring rooms
     * @param id Room ID
     * @return Vector of neighboring MapNode pointers
     */
    std::vector<MapNode*> get_neighbors(int id) {
        MapNode* node = get_node_by_id(id);
        if (!node) {
            return {};
        }
        
        return graph_.get_neighbors(node);
    }

    /**
     * @brief Get total number of rooms
     * @return Room count
     */
    size_t get_num_rooms() const {
        return nodes_.size();
    }

    /**
     * @brief Display detailed room information
     * @param id Room ID
     */
    void display_room_details(int id) const {
        if (id < 0 || id >= static_cast<int>(nodes_.size())) {
            std::cout << "Невірний номер кімнати!" << std::endl;
            return;
        }

        const auto& node = nodes_[id];
        
        std::cout << "\n┌────────────────────────────────────────┐" << std::endl;
        std::cout << "│  Кімната " << id << ": " << node->get_description() << std::endl;
        std::cout << "└────────────────────────────────────────┘" << std::endl;
        
        if (node->has_enemy()) {
            std::cout << "\n⚔️  ЗУСТРІЧ З ВОРОГОМ!" << std::endl;
            node->get_enemy()->display_stats();
        }
        
        if (node->has_item()) {
            std::cout << "\n💎 ЗНАЙДЕНО ПРЕДМЕТ!" << std::endl;
            node->get_item()->display_info();
        }
        
        auto neighbors = graph_.get_neighbors(node.get());
        std::cout << "\n🚪 Виходи: ";
        if (neighbors.empty()) {
            std::cout << "Немає (Глухий кут!)";
        } else {
            for (size_t i = 0; i < neighbors.size(); ++i) {
                std::cout << neighbors[i]->get_id();
                if (i < neighbors.size() - 1) {
                    std::cout << ", ";
                }
            }
        }
        std::cout << "\n" << std::endl;
    }
};

bool GameMap::random_initialized_ = false;

#endif // GAMEMAP_HPP

