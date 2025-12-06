#ifndef MAPNODE_HPP
#define MAPNODE_HPP

#include <string>

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
        : id_(id), description_(description), enemy_(enemy), item_(item) {
    }

    ~MapNode() = default;

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

    bool operator==(const MapNode& other) const {
        return id_ == other.id_;
    }
};

// Хеш для роботи з Graph (якщо він використовує unordered_map)
namespace std {
    template <>
    struct hash<MapNode*> {
        size_t operator()(MapNode* const& node) const {
            return hash<int>()(node->get_id());
        }
    };
}

#endif // MAPNODE_HPP