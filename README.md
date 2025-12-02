# Dungeon Crawler - C++17 Game Project

Лабораторна робота з об'єктно-орієнтованого програмування на C++.

Проєкт демонструє:
- Шаблонний клас `Graph<T>` з алгоритмами BFS/DFS
- Ієрархію класів персонажів з абстракцією та поліморфізмом
- Систему предметів з чисто віртуальними методами
- Процедурну генерацію ігрових карт
- Інтеграцію всіх систем у повноцінну гру

## Структура проєкту

```
lab-games/
├── CMakeLists.txt           # Конфігурація збірки
├── include/
│   ├── Graph.hpp            # Шаблонний граф (adjacency list)
│   ├── Character.hpp        # Абстрактний базовий клас
│   ├── Player.hpp           # Абстрактний клас гравця
│   ├── Warrior.hpp          # Клас воїна (120% damage)
│   ├── Mage.hpp             # Клас мага (ignores defense)
│   ├── Archer.hpp           # Клас лучника (30% crit chance)
│   ├── Enemy.hpp            # Абстрактний клас ворога
│   ├── Goblin.hpp           # Слабкий ворог
│   ├── Orc.hpp              # Сильний ворог (110% damage)
│   ├── Wraith.hpp           # Привид (50% physical resist, life drain)
│   ├── Item.hpp             # Абстрактний базовий клас предметів
│   ├── Weapon.hpp           # Зброя (boosts attack)
│   ├── Armor.hpp            # Броня (boosts defense)
│   ├── Potion.hpp           # Зілля (heals HP)
│   ├── MapNode.hpp          # Кімната підземелля
│   ├── GameMap.hpp          # Процедурна генерація карт
│   └── Game.hpp             # Головний ігровий клас
├── src/
│   └── main.cpp             # Точка входу в гру
└── README.md
```

## Збірка та запуск

### Використовуючи CMake:

```bash
# Створити директорію для збірки
mkdir build
cd build

# Згенерувати проєкт
cmake ..

# Зібрати
cmake --build .

# Запустити гру
./dungeon_game
```

### Ручна компіляція:

```bash
# Компіляція з C++
c++ -std=c++17 -Wall -Wextra -pedantic -I./include -o dungeon_game src/main.cpp

# Запуск
./dungeon_game
```

## Реалізовані компоненти

### 1. Graph<T> - Універсальний граф

Шаблонний клас для роботи з графами:
- **Структура**: список суміжності `unordered_map<T, unordered_set<T>>`
- **Методи**: `add_node()`, `add_edge()`, `add_undirected_edge()`, `get_neighbors()`
- **Алгоритми**: 
  - `bfs(start, end)` - пошук найкоротшого шляху
  - `dfs(start, end)` - пошук в глибину

```cpp
Graph<int> graph;
graph.add_node(1);
graph.add_node(2);
graph.add_undirected_edge(1, 2);
auto path = graph.bfs(1, 2);  // Найкоротший шлях
```

### 2. Character Hierarchy - Ієрархія персонажів

```
Character (abstract)
├── Player (abstract + inventory)
│   ├── Warrior  (HP: 150, ATK: 25, DEF: 12) - 120% damage
│   ├── Mage     (HP: 80,  ATK: 30, DEF: 5)  - ignores defense
│   └── Archer   (HP: 100, ATK: 20, DEF: 8)  - 30% crit (2x damage)
└── Enemy (abstract)
    ├── Goblin   (HP: 40,  ATK: 10, DEF: 3)  - weak
    ├── Orc      (HP: 100, ATK: 20, DEF: 10) - 110% damage
    └── Wraith   (HP: 60,  ATK: 18, DEF: 5)  - 50% resist, life drain
```

**Ключові особливості:**
- Абстрактний метод `attack(Character& target)` - кожен клас реалізує по-своєму
- Віртуальний метод `take_damage()` - Wraith перевизначає для опору
- Захищені поля доступні нащадкам
- Система інвентаря в Player

### 3. Item System - Система предметів

```
Item (abstract)
├── Weapon (boosts attack power)
├── Armor  (boosts defense)
└── Potion (restores HP)
```

**Поліморфізм:**
- Чисто віртуальний метод `use(Character*)`
- Різні ефекти для різних типів
- Інтеграція з Character через модифікацію характеристик

```cpp
Warrior warrior("Hero");
Weapon sword("Excalibur", "Legendary", 50);
sword.use(&warrior);  // Збільшує attack

Potion health("Potion", "Healing", 60);
health.use(&warrior);  // Відновлює HP
```

### 4. GameMap - Процедурна генерація підземелля

- Використовує `Graph<MapNode*>` для структури
- Автоматична генерація кімнат з ворогами та предметами
- Гарантована зв'язність (всі кімнати досяжні)
- Пошук шляху через BFS

```cpp
GameMap dungeon;
dungeon.generate_map(10, 4, 5);  // 10 кімнат, 4 вороги, 5 предметів
dungeon.print_map();

auto path = dungeon.find_path(0, 9);  // Найкоротший шлях до виходу
```

### 5. Game - Головний ігровий клас

- `initialize()` - створення персонажа та генерація підземелля
- `game_loop()` - покроковий ігровий цикл
- Обробка помилок через try-catch
- Інтерактивне меню та управління

## Принципи ООП

### Інкапсуляція
- Приватні/захищені поля
- Публічні методи доступу

### Наслідування
- 3-рівнева ієрархія: `Character → Player → Warrior/Mage/Archer`
- Повторне використання коду в базових класах

### Поліморфізм
- Чисто віртуальні методи: `attack()`, `use()`
- Віртуальні деструктори
- Динамічна диспетчеризація через `Character*`, `Item*`

### Абстракція
- Абстрактні класи: `Character`, `Player`, `Enemy`, `Item`
- Визначення контрактів для похідних класів

## Ключові технології C++17

- ✅ Templates: `Graph<T>`
- ✅ STL containers: `vector`, `unordered_map`, `unordered_set`, `queue`, `stack`
- ✅ Smart pointers: `unique_ptr`, `shared_ptr`
- ✅ Virtual functions & abstract classes
- ✅ Exception handling
- ✅ `<random>` library
- ✅ Range-based for loops
- ✅ `auto` type deduction

## Геймплей

1. Виберіть клас персонажа (Warrior/Mage/Archer)
2. Досліджуйте процедурно згенероване підземелля
3. Перемагайте ворогів у покрокових боях
4. Збирайте зброю, броню та зілля
5. Знайдіть вихід і виживіть!

**Кожна гра унікальна завдяки процедурній генерації!**

## Автор

Лабораторна робота з ООП, C++17
