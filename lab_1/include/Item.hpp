#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <iostream>

class Character;

/**
 * @brief Abstract base class for all items
 */
class Item {
protected:
    std::string name_;         ///< Item name
    std::string description_;  ///< Item description

public:
    /**
     * @brief Construct a new Item
     * @param name Item name
     * @param description Item description
     */
    Item(const std::string& name, const std::string& description)
        : name_(name), description_(description) {}

    virtual ~Item() = default;

    /**
     * @brief Use the item on a character (pure virtual)
     * @param character Target character
     */
    virtual void use(Character* character) = 0;

    /** @brief Get item name */
    std::string get_name() const { return name_; }
    
    /** @brief Get item description */
    std::string get_description() const { return description_; }

    /**
     * @brief Display item information
     */
    virtual void display_info() const {
        std::cout << "Предмет: " << name_ << std::endl;
        std::cout << "Опис: " << description_ << std::endl;
    }
};

#endif // ITEM_HPP

