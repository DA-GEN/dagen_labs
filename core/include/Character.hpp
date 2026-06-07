#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <algorithm> // для std::max

// Тип шкоди визначає, чи застосовується звичайний захист персонажа.
// Фізична шкода проходить крізь броню (Character::defense_), магічна - ігнорує її.
// Введення цього типу дозволяє Mage коректно завдавати шкоду через єдиний
// поліморфний інтерфейс take_damage замість прямого виклику set_hp.
enum class DamageType {
    Physical,
    Magical
};

class Character {
protected:
    std::string name_;
    int hp_;
    int max_hp_;
    int attack_power_;
    int defense_;

    // Шаблонний метод (Template Method): обчислює, скільки шкоди фактично
    // дійде до персонажа з урахуванням його захисту/резистів.
    // Базова реалізація враховує лише стандартний захист і лише для
    // фізичної шкоди; підкласи (напр. Wraith) можуть перевизначити цей хук,
    // додаючи власні модифікатори (резист, вразливість тощо), не дублюючи
    // при цьому решту логіки take_damage.
    virtual int apply_defense(int amount, DamageType type) const {
        if (type == DamageType::Magical) {
            return amount; // магія ігнорує фізичний захист
        }

        int actual_damage = amount - defense_;
        if (actual_damage < 1) {
            actual_damage = 1;
        }
        return actual_damage;
    }

public:
    Character(const std::string& name, int max_hp, int attack_power, int defense)
        : name_(name), hp_(max_hp), max_hp_(max_hp),
        attack_power_(attack_power), defense_(defense) {
    }

    virtual ~Character() = default;

    // ЗМІНА: Повертає опис атаки
    virtual std::string attack(Character& target) = 0;

    // ЗМІНА: Повертає лог отримання пошкоджень
    virtual std::string take_damage(int amount, DamageType type = DamageType::Physical) {
        if (amount <= 0) return name_ + " не отримує пошкоджень.";

        int actual_damage = apply_defense(amount, type);

        hp_ -= actual_damage;
        if (hp_ < 0) {
            hp_ = 0;
        }

        return name_ + " отримує " + std::to_string(actual_damage) + " шкоди! (HP: " + std::to_string(hp_) + "/" + std::to_string(max_hp_) + ")";
    }

    bool is_alive() const {
        return hp_ > 0;
    }

    // ЗМІНА: Повертає лог лікування
    std::string heal(int amount) {
        if (amount <= 0) return "";

        int old_hp = hp_;
        hp_ += amount;
        if (hp_ > max_hp_) {
            hp_ = max_hp_;
        }

        int healed_amount = hp_ - old_hp;

        return name_ + " відновлює " + std::to_string(healed_amount) + " HP. (HP: " + std::to_string(hp_) + "/" + std::to_string(max_hp_) + ")";
    }

    // ЗМІНА: Повертає форматований рядок статистики
    virtual std::string get_stats_string() const {
        return name_ + " | HP: " + std::to_string(hp_) + "/" + std::to_string(max_hp_) +
            " | Атака: " + std::to_string(attack_power_) +
            " | Захист: " + std::to_string(defense_);
    }

    std::string get_name() const { return name_; }
    int get_hp() const { return hp_; }
    int get_max_hp() const { return max_hp_; }
    int get_attack_power() const { return attack_power_; }
    int get_defense() const { return defense_; }

    // Використовується системою предметів
    void set_hp(int hp) {
        hp_ = hp;
        if (hp_ > max_hp_) hp_ = max_hp_;
        if (hp_ < 0) hp_ = 0;
    }

    void modify_attack_power(int delta) { attack_power_ += delta; }
    void modify_defense(int delta) { defense_ += delta; }
};

#endif // CHARACTER_HPP
