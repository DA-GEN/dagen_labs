#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "Graph.hpp"
#include "MapNode.hpp"
#include "Enemy.hpp"
#include "Item.hpp"
#include "RandomProvider.hpp"
#include "EnemyFactory.hpp"
#include "ItemFactory.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <string>

class GameMap {
private:
    Graph<MapNode*> graph_;
    std::vector<std::unique_ptr<MapNode>> nodes_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Item>> items_;

    // ЗМІНА: карта більше не створює власний генератор випадкових чисел і
    // не знає, які конкретно класи ворогів/предметів існують. Усі ці
    // залежності надходять ззовні (Dependency Injection), що прибирає
    // дублювання std::rand-ініціалізації та усуває порушення OCP.
    IRandomProvider& rng_;
    const IEnemyFactory& enemy_factory_;
    const IItemFactory& item_factory_;

    std::string generate_room_description() {
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

        int type_idx = rng_.next_int(0, static_cast<int>(room_types.size()) - 1);
        int feature_idx = rng_.next_int(0, static_cast<int>(features.size()) - 1);

        return room_types[static_cast<size_t>(type_idx)] + " " + features[static_cast<size_t>(feature_idx)];
    }

public:
    GameMap(IRandomProvider& rng, const IEnemyFactory& enemy_factory, const IItemFactory& item_factory)
        : rng_(rng), enemy_factory_(enemy_factory), item_factory_(item_factory) {
    }

    ~GameMap() = default;

    // Перевіряє, чи всі вороги мертві (або їх взагалі не лишилося)
    bool allEnemiesDefeated() const {
        for (const auto& node : nodes_) {
            if (node->has_enemy()) {
                if (node->get_enemy()->is_alive()) {
                    return false; // Знайшли живого ворога -> гра ще не виграна
                }
            }
        }
        return true; // Нікого не знайшли -> перемога
    }

    void generate_map(int num_rooms, int num_enemies, int num_items) {
        nodes_.clear();
        enemies_.clear();
        items_.clear();
        graph_ = Graph<MapNode*>();

        for (int i = 0; i < num_rooms; ++i) {
            std::string desc = generate_room_description();
            auto node = std::make_unique<MapNode>(i, desc);
            graph_.add_node(node.get());
            nodes_.push_back(std::move(node));
        }

        // Лінійний шлях
        for (int i = 0; i < num_rooms - 1; ++i) {
            graph_.add_undirected_edge(nodes_[static_cast<size_t>(i)].get(), nodes_[static_cast<size_t>(i + 1)].get());
        }

        // Випадкові з'єднання
        int extra_connections = num_rooms / 2;
        for (int i = 0; i < extra_connections; ++i) {
            int from = rng_.next_int(0, num_rooms - 1);
            int to = rng_.next_int(0, num_rooms - 1);
            if (from != to && !graph_.has_edge(nodes_[static_cast<size_t>(from)].get(), nodes_[static_cast<size_t>(to)].get())) {
                graph_.add_undirected_edge(nodes_[static_cast<size_t>(from)].get(), nodes_[static_cast<size_t>(to)].get());
            }
        }

        // Розміщення ворогів і предметів
        std::vector<int> available_rooms(static_cast<size_t>(num_rooms));
        for (int i = 0; i < num_rooms; ++i) available_rooms[static_cast<size_t>(i)] = i;

        shuffle(available_rooms);
        for (int i = 0; i < num_enemies && i < num_rooms; ++i) {
            auto enemy = enemy_factory_.create_random();
            nodes_[static_cast<size_t>(available_rooms[static_cast<size_t>(i)])]->set_enemy(enemy.get());
            enemies_.push_back(std::move(enemy));
        }

        shuffle(available_rooms);
        for (int i = 0; i < num_items && i < num_rooms; ++i) {
            auto item = item_factory_.create_random();
            nodes_[static_cast<size_t>(available_rooms[static_cast<size_t>(i)])]->set_item(item.get());
            items_.push_back(std::move(item));
        }
    }

    MapNode* get_node_by_id(int id) {
        if (id >= 0 && id < static_cast<int>(nodes_.size())) {
            return nodes_[static_cast<size_t>(id)].get();
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

private:
    // Фішер-Єйтс перемішування на основі ін'єктованого rng_ (замість
    // std::shuffle зі std::random_device/std::mt19937, які тут створювались
    // окремо при кожному виклику).
    void shuffle(std::vector<int>& values) {
        for (size_t i = values.size(); i > 1; --i) {
            size_t j = static_cast<size_t>(rng_.next_int(0, static_cast<int>(i) - 1));
            std::swap(values[i - 1], values[j]);
        }
    }
};

#endif // GAMEMAP_HPP
