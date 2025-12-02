#ifndef MAPNODE_HPP
#define MAPNODE_HPP

#include <string>
#include <iostream>

class Enemy;
class Item;

/**
 * @brief Represents a room/location in the dungeon map
 */
class MapNode {
private:
    int id_;                   ///< Room ID
    std::string description_;  ///< Room description
    Enemy* enemy_;             ///< Non-owning pointer to enemy
    Item* item_;               ///< Non-owning pointer to item

public:
    /**
     * @brief Construct a new MapNode
     * @param id Room ID
     * @param description Room description
     * @param enemy Pointer to enemy (optional)
     * @param item Pointer to item (optional)
     */
    MapNode(int id, const std::string& description, Enemy* enemy = nullptr, Item* item = nullptr)
        : id_(id), description_(description), enemy_(enemy), item_(item) {}

    ~MapNode() = default;

    /** @brief Get room ID */
    int get_id() const { return id_; }
    
    /** @brief Get room description */
    std::string get_description() const { return description_; }
    
    /** @brief Get enemy pointer */
    Enemy* get_enemy() const { return enemy_; }
    
    /** @brief Get item pointer */
    Item* get_item() const { return item_; }

    /** @brief Set room description */
    void set_description(const std::string& desc) { description_ = desc; }
    
    /** @brief Set enemy */
    void set_enemy(Enemy* enemy) { enemy_ = enemy; }
    
    /** @brief Set item */
    void set_item(Item* item) { item_ = item; }

    /** @brief Check if room has enemy */
    bool has_enemy() const { return enemy_ != nullptr; }
    
    /** @brief Check if room has item */
    bool has_item() const { return item_ != nullptr; }

    /** @brief Clear enemy from room */
    void clear_enemy() { enemy_ = nullptr; }
    
    /** @brief Clear item from room */
    void clear_item() { item_ = nullptr; }

    /**
     * @brief Display room information
     */
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

    /**
     * @brief Equality operator
     * @param other Other MapNode to compare
     * @return true if IDs match
     */
    bool operator==(const MapNode& other) const {
        return id_ == other.id_;
    }
};

/**
 * @brief Hash function for MapNode pointers
 */
namespace std {
    template <>
    struct hash<MapNode*> {
        size_t operator()(MapNode* const& node) const {
            return hash<int>()(node->get_id());
        }
    };
}

#endif // MAPNODE_HPP

