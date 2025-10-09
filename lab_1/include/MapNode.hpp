#ifndef MAPNODE_HPP
#define MAPNODE_HPP

#include <string>
#include <iostream>

class Enemy;
class Item;

class MapNode {
private:
    int id_;
    std::string description_;
    Enemy* enemy_;  // Non-owning pointer
    Item* item_;    // Non-owning pointer

public:
    MapNode(int id, const std::string& description, Enemy* enemy = nullptr, Item* item = nullptr)
        : id_(id), description_(description), enemy_(enemy), item_(item) {}

    ~MapNode() = default;  // Doesn't delete enemy/item - GameMap owns them

    int get_id() const { return id_; }
    std::string get_description() const { return description_; }
    Enemy* get_enemy() const { return enemy_; }
    Item* get_item() const { return item_; }

    void set_description(const std::string& desc) { description_ = desc; }
    void set_enemy(Enemy* enemy) { enemy_ = enemy; }
    void set_item(Item* item) { item_ = item; }

    bool has_enemy() const { return enemy_ != nullptr; }
    bool has_item() const { return item_ != nullptr; }

    void clear_enemy() { enemy_ = nullptr; }
    void clear_item() { item_ = nullptr; }

    void display() const {
        std::cout << "[Кімната " << id_ << "] " << description_ << std::endl;
        
        if (has_enemy()) {
            std::cout << "  ⚔️  Тут є ворог!" << std::endl;
        }
        
        if (has_item()) {
            std::cout << "  💎 Тут є предмет!" << std::endl;
        }
        
        if (!has_enemy() && !has_item()) {
            std::cout << "  (Порожня кімната)" << std::endl;
        }
    }

    bool operator==(const MapNode& other) const {
        return id_ == other.id_;
    }
};

// Required for Graph<MapNode*> to work with unordered containers
namespace std {
    template <>
    struct hash<MapNode*> {
        size_t operator()(MapNode* const& node) const {
            return hash<int>()(node->get_id());
        }
    };
}

#endif // MAPNODE_HPP

