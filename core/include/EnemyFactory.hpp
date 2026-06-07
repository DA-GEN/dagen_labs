#ifndef ENEMY_FACTORY_HPP
#define ENEMY_FACTORY_HPP

#include "Enemy.hpp"
#include "Goblin.hpp"
#include "Orc.hpp"
#include "Wraith.hpp"
#include "RandomProvider.hpp"
#include <memory>
#include <string>

// Factory Method: інкапсулює знання про конкретні класи ворогів.
// Раніше це знання (switch по типу + список конструкторів) "протікало"
// у GameMap::create_random_enemy, через що додавання нового типу ворога
// вимагало змін у класі карти (порушення OCP). Тепер GameMap залежить
// лише від абстракції IEnemyFactory.
class IEnemyFactory {
public:
    virtual ~IEnemyFactory() = default;
    virtual std::unique_ptr<Enemy> create_random() const = 0;
};

class EnemyFactory : public IEnemyFactory {
public:
    explicit EnemyFactory(IRandomProvider& rng) : rng_(rng) {}

    std::unique_ptr<Enemy> create_random() const override {
        switch (rng_.next_int(0, 2)) {
        case 0: return std::make_unique<Goblin>("Гоблін");
        case 1: return std::make_unique<Orc>("Орк");
        default: return std::make_unique<Wraith>("Примара");
        }
    }

private:
    IRandomProvider& rng_;
};

#endif // ENEMY_FACTORY_HPP
