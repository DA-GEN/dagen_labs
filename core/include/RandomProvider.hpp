#ifndef RANDOM_PROVIDER_HPP
#define RANDOM_PROVIDER_HPP

#include <random>

// Абстракція над джерелом випадковості.
// Дозволяє підставляти у Gameplay-класи (GameMap, Archer тощо) як справжній
// генератор, так і детермінований "фейк" у юніт-тестах (Dependency Inversion).
class IRandomProvider {
public:
    virtual ~IRandomProvider() = default;

    // Повертає ціле число у діапазоні [min, max] (включно).
    virtual int next_int(int min, int max) = 0;

    // Зручний хелпер: true з імовірністю percent_chance відсотків (0-100).
    bool roll_chance(int percent_chance) {
        return next_int(0, 99) < percent_chance;
    }
};

// Єдина реалізація на основі <random>, яку використовує застосунок.
// Інкапсулює ініціалізацію генератора - раніше кожен клас (GameMap, Archer)
// окремо викликав std::srand(std::time(nullptr)), дублюючи код і роблячи
// поведінку залежною від глобального стану.
class RandomProvider : public IRandomProvider {
public:
    RandomProvider() : engine_(std::random_device{}()) {}
    explicit RandomProvider(unsigned seed) : engine_(seed) {}

    int next_int(int min, int max) override {
        if (min > max) std::swap(min, max);
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine_);
    }

private:
    std::mt19937 engine_;
};

#endif // RANDOM_PROVIDER_HPP
