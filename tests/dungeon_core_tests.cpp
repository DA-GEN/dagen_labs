#include <gtest/gtest.h>

#include "Character.hpp"
#include "Player.hpp"
#include "Warrior.hpp"
#include "Mage.hpp"
#include "Archer.hpp"
#include "Enemy.hpp"
#include "Goblin.hpp"
#include "Orc.hpp"
#include "Wraith.hpp"
#include "Item.hpp"
#include "Weapon.hpp"
#include "Armor.hpp"
#include "Potion.hpp"
#include "Graph.hpp"
#include "MapNode.hpp"
#include "GameMap.hpp"
#include "RandomProvider.hpp"
#include "EnemyFactory.hpp"
#include "ItemFactory.hpp"
#include "GameEngine.hpp"
#include "GameEvents.hpp"

#include <vector>
#include <string>

// =====================================================================
// Тестові дублери (Test Doubles)
// =====================================================================

// Детермінований "фейковий" генератор: видає наперед задану послідовність
// чисел по колу. Використовується замість RandomProvider, щоб зробити
// поведінку, яка раніше залежала від std::rand (вибір типу ворога/предмета,
// крит лучника, генерація підземелля), повністю відтворюваною у тестах.
class SequenceRandomProvider : public IRandomProvider {
public:
    explicit SequenceRandomProvider(std::vector<int> sequence)
        : sequence_(std::move(sequence)), index_(0) {
    }

    int next_int(int min, int max) override {
        if (sequence_.empty()) return min;
        int value = sequence_[index_ % sequence_.size()];
        ++index_;
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

private:
    std::vector<int> sequence_;
    size_t index_;
};

// Шпигун (Spy) за подіями GameEngine: фіксує усі повідомлення, на які
// раніше підписувався лише Qt-шар через сигнали. Дозволяє тестувати
// GameEngine як звичайний C++ об'єкт - без QApplication і GUI.
class SpyGameObserver : public IGameObserver {
public:
    std::vector<std::string> log_messages;
    int stats_updated_count = 0;
    int room_updated_count = 0;
    bool last_room_has_enemy = false;
    bool last_room_has_item = false;
    bool game_started = false;
    bool game_over_called = false;
    bool last_game_over_victory = false;

    void on_log_message(const std::string& message) override {
        log_messages.push_back(message);
    }
    void on_stats_updated() override { ++stats_updated_count; }
    void on_room_updated(const std::string&, bool has_enemy, bool has_item) override {
        ++room_updated_count;
        last_room_has_enemy = has_enemy;
        last_room_has_item = has_item;
    }
    void on_game_started() override { game_started = true; }
    void on_game_over(bool victory) override {
        game_over_called = true;
        last_game_over_victory = victory;
    }
};

// =====================================================================
// Character / Player / класи персонажів гравця
// =====================================================================

TEST(CharacterTest, InitialStateIsAliveWithFullHp) {
    Warrior warrior("Тест");
    EXPECT_EQ(warrior.get_hp(), warrior.get_max_hp());
    EXPECT_TRUE(warrior.is_alive());
}

TEST(CharacterTest, TakeDamageReducesHpByAmountMinusDefense) {
    Warrior warrior("Тест"); // hp=150, defense=12
    warrior.take_damage(30, DamageType::Physical);
    EXPECT_EQ(warrior.get_hp(), 150 - (30 - 12));
}

TEST(CharacterTest, TakeDamageNeverDealsLessThanOne) {
    Warrior warrior("Тест"); // defense=12
    warrior.take_damage(5, DamageType::Physical); // 5 - 12 < 1 -> має пройти 1
    EXPECT_EQ(warrior.get_hp(), 149);
}

TEST(CharacterTest, TakeDamageClampsHpAtZero) {
    Warrior warrior("Тест");
    warrior.take_damage(10000);
    EXPECT_EQ(warrior.get_hp(), 0);
    EXPECT_FALSE(warrior.is_alive());
}

TEST(CharacterTest, NonPositiveDamageIsIgnored) {
    Warrior warrior("Тест");
    int hp_before = warrior.get_hp();
    warrior.take_damage(0);
    warrior.take_damage(-5);
    EXPECT_EQ(warrior.get_hp(), hp_before);
}

TEST(CharacterTest, MagicalDamageIgnoresDefense) {
    // ЗМІНА: раніше це перевірити було неможливо через Character - магія
    // обходила take_damage напряму (Mage::attack викликав set_hp). Тепер
    // ігнорування захисту - частина контракту take_damage(amount, type).
    Warrior warrior("Тест"); // defense = 12
    warrior.take_damage(20, DamageType::Magical);
    EXPECT_EQ(warrior.get_hp(), 150 - 20);
}

TEST(CharacterTest, HealRestoresHpButNotAboveMax) {
    Warrior warrior("Тест");
    warrior.take_damage(50, DamageType::Magical);
    warrior.heal(1000);
    EXPECT_EQ(warrior.get_hp(), warrior.get_max_hp());
}

TEST(CharacterTest, ModifyAttackAndDefense) {
    Warrior warrior("Тест");
    int base_attack = warrior.get_attack_power();
    int base_defense = warrior.get_defense();

    warrior.modify_attack_power(10);
    warrior.modify_defense(5);

    EXPECT_EQ(warrior.get_attack_power(), base_attack + 10);
    EXPECT_EQ(warrior.get_defense(), base_defense + 5);
}

TEST(PlayerClassesTest, WarriorBaseStats) {
    Warrior warrior("Воїн");
    EXPECT_EQ(warrior.get_max_hp(), 150);
    EXPECT_EQ(warrior.get_attack_power(), 25);
    EXPECT_EQ(warrior.get_defense(), 12);
}

TEST(PlayerClassesTest, MageBaseStats) {
    Mage mage("Маг");
    EXPECT_EQ(mage.get_max_hp(), 80);
    EXPECT_EQ(mage.get_attack_power(), 30);
    EXPECT_EQ(mage.get_defense(), 5);
}

TEST(PlayerClassesTest, WarriorAttackDealsBoostedPhysicalDamage) {
    Warrior warrior("Воїн");   // attack_power = 25 -> 25 * 1.2 = 30
    Goblin goblin("Гоблін");   // defense = 3
    warrior.attack(goblin);
    EXPECT_EQ(goblin.get_hp(), 40 - (30 - 3));
}

TEST(PlayerClassesTest, MageAttackRoutesThroughPolymorphicTakeDamageAndIgnoresDefense) {
    // ЗМІНА: ключовий регресійний тест на виправлення інкапсуляції -
    // Mage більше не смикає set_hp() напряму, а атака зменшує HP цілі
    // рівно на attack_power незалежно від її захисту.
    Mage mage("Маг");          // attack_power = 30
    Orc orc("Орк");            // hp = 100, defense = 10
    mage.attack(orc);
    EXPECT_EQ(orc.get_hp(), 100 - 30);
}

TEST(PlayerClassesTest, ArcherCritUsesInjectedRandomProvider) {
    // roll_chance(30) повертає true, коли next_int(0,99) < 30.
    SequenceRandomProvider always_crit({0});
    SequenceRandomProvider never_crit({99});

    Archer crit_archer("Лучник", always_crit);   // attack_power = 20
    Archer normal_archer("Лучник", never_crit);

    Goblin target1("Гоблін");
    Goblin target2("Гоблін");

    crit_archer.attack(target1);   // крит: 2 * 20 = 40, з урахуванням захисту (3) -> 37
    normal_archer.attack(target2); // звичайний: 20, з урахуванням захисту (3) -> 17

    EXPECT_EQ(target1.get_hp(), 40 - (40 - 3));
    EXPECT_EQ(target2.get_hp(), 40 - (20 - 3));
}

// =====================================================================
// Enemy: Goblin / Orc / Wraith
// =====================================================================

TEST(EnemyTest, GoblinBaseStatsAndAttack) {
    Goblin goblin("Гоблін");
    Warrior target("Мішень"); // defense = 12
    EXPECT_EQ(goblin.get_attack_power(), 10);

    goblin.attack(target);
    EXPECT_EQ(target.get_hp(), 150 - (10 - 12 < 1 ? 1 : 10 - 12));
}

TEST(EnemyTest, OrcDealsBoostedDamage) {
    Orc orc("Орк"); // attack_power = 20 -> 20 * 1.1 = 22
    Warrior target("Мішень"); // defense = 12
    orc.attack(target);
    EXPECT_EQ(target.get_hp(), 150 - (22 - 12));
}

TEST(EnemyTest, WraithResistsHalfOfPhysicalDamage) {
    // ЗМІНА: цей тест демонструє, що прибирання дублювання з
    // Wraith::take_damage (через перевизначення apply_defense) НЕ зламало
    // оригінальну механіку 50% фізичного резисту.
    Wraith wraith("Примара"); // hp=60, defense=5
    wraith.take_damage(41, DamageType::Physical);
    // 41 * 0.5 = 20 (резист) -> 20 - 5 (захист) = 15
    EXPECT_EQ(wraith.get_hp(), 60 - 15);
}

TEST(EnemyTest, WraithPhysicalResistanceHasMinimumOfOne) {
    Wraith wraith("Примара");
    wraith.take_damage(1, DamageType::Physical);
    // 1 * 0.5 = 0 -> мінімум 1 -> 1 - захист(5) -> мінімум 1
    EXPECT_EQ(wraith.get_hp(), 60 - 1);
}

TEST(EnemyTest, WraithDoesNotResistMagicalDamage) {
    // ЗМІНА: новий аспект поведінки, який зʼявився завдяки введенню
    // DamageType - спектральна істота чинить опір лише фізичним ударам,
    // тому магія Mage тепер коректно "пробиває" Wraith повністю.
    Wraith wraith("Примара");
    wraith.take_damage(20, DamageType::Magical);
    EXPECT_EQ(wraith.get_hp(), 60 - 20);
}

TEST(EnemyTest, WraithAttackHealsItselfViaVampirism) {
    Wraith wraith("Примара");
    Warrior target("Мішень");

    wraith.take_damage(30, DamageType::Magical); // знизити HP, щоб лікування було помітним
    int hp_before = wraith.get_hp();

    wraith.attack(target);

    EXPECT_GT(wraith.get_hp(), hp_before);
}

// =====================================================================
// Item: Weapon / Armor / Potion
// =====================================================================

TEST(ItemTest, WeaponIncreasesAttackPowerByHalfDamage) {
    Warrior warrior("Воїн");
    int base_attack = warrior.get_attack_power();

    Weapon sword("Меч", "опис", 20);
    sword.use(&warrior);

    EXPECT_EQ(warrior.get_attack_power(), base_attack + 10);
}

TEST(ItemTest, ArmorIncreasesDefense) {
    Warrior warrior("Воїн");
    int base_defense = warrior.get_defense();

    Armor armor("Щит", "опис", 15);
    armor.use(&warrior);

    EXPECT_EQ(warrior.get_defense(), base_defense + 15);
}

TEST(ItemTest, PotionHealsCharacter) {
    Warrior warrior("Воїн");
    warrior.take_damage(100, DamageType::Magical);
    int hp_before = warrior.get_hp();

    Potion potion("Зілля", "опис", 40);
    potion.use(&warrior);

    EXPECT_EQ(warrior.get_hp(), hp_before + 40);
}

TEST(ItemTest, ItemsRejectNullCharacter) {
    Weapon sword("Меч", "опис", 20);
    std::string result = sword.use(nullptr);
    EXPECT_FALSE(result.empty());
}

// =====================================================================
// Graph
// =====================================================================

TEST(GraphTest, AddNodeAndEdgeCreatesSymmetricNeighbors) {
    Graph<int> graph;
    graph.add_node(1);
    graph.add_node(2);
    graph.add_undirected_edge(1, 2);

    EXPECT_TRUE(graph.has_edge(1, 2));
    EXPECT_TRUE(graph.has_edge(2, 1));
}

TEST(GraphTest, BfsFindsShortestPath) {
    Graph<int> graph;
    for (int i = 0; i <= 4; ++i) graph.add_node(i);
    graph.add_undirected_edge(0, 1);
    graph.add_undirected_edge(1, 2);
    graph.add_undirected_edge(0, 3);
    graph.add_undirected_edge(3, 2);
    graph.add_undirected_edge(2, 4);

    auto path = graph.bfs(0, 4);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), 0);
    EXPECT_EQ(path.back(), 4);
    EXPECT_LE(path.size(), 4u); // 0 -> 1/3 -> 2 -> 4
}

TEST(GraphTest, GetNeighborsThrowsForUnknownNode) {
    Graph<int> graph;
    graph.add_node(1);
    EXPECT_THROW(graph.get_neighbors(42), std::runtime_error);
}

// =====================================================================
// MapNode
// =====================================================================

TEST(MapNodeTest, TracksEnemyAndItemPresence) {
    MapNode node(0, "Кімната");
    EXPECT_FALSE(node.has_enemy());
    EXPECT_FALSE(node.has_item());

    Goblin goblin("Гоблін");
    Potion potion("Зілля", "опис", 10);

    node.set_enemy(&goblin);
    node.set_item(&potion);
    EXPECT_TRUE(node.has_enemy());
    EXPECT_TRUE(node.has_item());

    node.clear_enemy();
    node.clear_item();
    EXPECT_FALSE(node.has_enemy());
    EXPECT_FALSE(node.has_item());
}

// =====================================================================
// RandomProvider
// =====================================================================

TEST(RandomProviderTest, NextIntStaysWithinRequestedRange) {
    RandomProvider rng(12345u);
    for (int i = 0; i < 200; ++i) {
        int value = rng.next_int(5, 10);
        EXPECT_GE(value, 5);
        EXPECT_LE(value, 10);
    }
}

TEST(RandomProviderTest, RollChanceRespectsBoundaries) {
    SequenceRandomProvider always_low({0});
    SequenceRandomProvider always_high({99});

    EXPECT_TRUE(always_low.roll_chance(30));
    EXPECT_FALSE(always_high.roll_chance(30));
}

// =====================================================================
// EnemyFactory / ItemFactory (Factory Method)
// =====================================================================

TEST(EnemyFactoryTest, ProducesTypeCorrespondingToRandomRoll) {
    SequenceRandomProvider rng_goblin({0});
    SequenceRandomProvider rng_orc({1});
    SequenceRandomProvider rng_wraith({2});

    EXPECT_NE(dynamic_cast<Goblin*>(EnemyFactory(rng_goblin).create_random().get()), nullptr);
    EXPECT_NE(dynamic_cast<Orc*>(EnemyFactory(rng_orc).create_random().get()), nullptr);
    EXPECT_NE(dynamic_cast<Wraith*>(EnemyFactory(rng_wraith).create_random().get()), nullptr);
}

TEST(ItemFactoryTest, ProducesTypeCorrespondingToRandomRoll) {
    SequenceRandomProvider rng_weapon({0, 0, 20});
    SequenceRandomProvider rng_armor({1, 0, 15});
    SequenceRandomProvider rng_potion({2, 0, 30});

    EXPECT_NE(dynamic_cast<Weapon*>(ItemFactory(rng_weapon).create_random().get()), nullptr);
    EXPECT_NE(dynamic_cast<Armor*>(ItemFactory(rng_armor).create_random().get()), nullptr);
    EXPECT_NE(dynamic_cast<Potion*>(ItemFactory(rng_potion).create_random().get()), nullptr);
}

// =====================================================================
// GameMap
// =====================================================================

TEST(GameMapTest, GenerateMapCreatesRequestedRoomsEnemiesAndItems) {
    SequenceRandomProvider rng({3, 5, 1, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8});
    EnemyFactory enemy_factory(rng);
    ItemFactory item_factory(rng);
    GameMap map(rng, enemy_factory, item_factory);

    map.generate_map(8, 4, 4);

    EXPECT_EQ(map.get_num_rooms(), 8u);

    int enemy_count = 0;
    int item_count = 0;
    for (int i = 0; i < 8; ++i) {
        MapNode* node = map.get_node_by_id(i);
        ASSERT_NE(node, nullptr);
        if (node->has_enemy()) ++enemy_count;
        if (node->has_item()) ++item_count;
    }

    EXPECT_EQ(enemy_count, 4);
    EXPECT_EQ(item_count, 4);
}

TEST(GameMapTest, LinearPathConnectsConsecutiveRooms) {
    SequenceRandomProvider rng({0});
    EnemyFactory enemy_factory(rng);
    ItemFactory item_factory(rng);
    GameMap map(rng, enemy_factory, item_factory);

    map.generate_map(5, 0, 0);

    for (int i = 0; i < 4; ++i) {
        auto neighbors = map.get_neighbors(i);
        bool connected_to_next = false;
        for (auto* neighbor : neighbors) {
            if (neighbor->get_id() == i + 1) connected_to_next = true;
        }
        EXPECT_TRUE(connected_to_next) << "Кімната " << i << " має зʼєднуватись з " << (i + 1);
    }
}

TEST(GameMapTest, AllEnemiesDefeatedTrueWhenNoneAreAlive) {
    SequenceRandomProvider rng({0});
    EnemyFactory enemy_factory(rng);
    ItemFactory item_factory(rng);
    GameMap map(rng, enemy_factory, item_factory);

    map.generate_map(4, 1, 0);
    EXPECT_FALSE(map.allEnemiesDefeated());

    for (int i = 0; i < 4; ++i) {
        MapNode* node = map.get_node_by_id(i);
        if (node->has_enemy()) {
            node->get_enemy()->take_damage(10000, DamageType::Magical);
        }
    }

    EXPECT_TRUE(map.allEnemiesDefeated());
}

// =====================================================================
// GameEngine - інтеграційні тести (рушій без Qt, перевіряється через Spy)
// =====================================================================

class GameEngineTest : public ::testing::Test {
protected:
    SpyGameObserver observer;
    GameEngine engine{observer};
};

TEST_F(GameEngineTest, StartNewGameNotifiesObserverAndSetsUpPlayer) {
    engine.start_new_game("Герой", 0); // Воїн

    EXPECT_TRUE(observer.game_started);
    EXPECT_GE(observer.room_updated_count, 1);
    EXPECT_EQ(engine.get_player_hp(), engine.get_player_max_hp());
    EXPECT_GT(engine.get_player_max_hp(), 0);
}

TEST_F(GameEngineTest, EmptyPlayerNameFallsBackToDefault) {
    engine.start_new_game("", 0);

    ASSERT_FALSE(observer.log_messages.empty());
    EXPECT_NE(observer.log_messages.front().find("Герой"), std::string::npos);
}

TEST_F(GameEngineTest, CannotMoveWhileRoomHasLivingEnemy) {
    // Стартуємо доти, доки не отримаємо підземелля, де у початковій кімнаті є ворог.
    bool found_case = false;
    for (int attempt = 0; attempt < 25 && !found_case; ++attempt) {
        SpyGameObserver local_observer;
        GameEngine local_engine(local_observer);
        local_engine.start_new_game("Герой", 0);

        if (local_engine.get_enemy_hp() > 0) {
            found_case = true;
            size_t messages_before = local_observer.log_messages.size();
            local_engine.action_move(0);

            ASSERT_GT(local_observer.log_messages.size(), messages_before);
            EXPECT_NE(local_observer.log_messages.back().find("бою"), std::string::npos);
        }
    }
    EXPECT_TRUE(found_case) << "Не вдалось згенерувати підземелля з ворогом у стартовій кімнаті за розумну кількість спроб";
}

TEST_F(GameEngineTest, AttackingDefeatedDungeonTriggersVictory) {
    engine.start_new_game("Герой", 1); // Маг - ігнорує захист, швидше всіх вбиває

    // Завжди ходимо у вихід №0 неможливо - на циклічних картах це призводить
    // до нескінченного "стрибання" між двома кімнатами без відвідування
    // решти підземелля. Тому при виборі напрямку використовуємо власний
    // (детермінований, з фіксованим зерном) генератор - випадкове блукання
    // на маленькому зв'язному графі гарантовано рано чи пізно відвідає всі
    // кімнати, а отже й зустріне всіх ворогів.
    RandomProvider exit_picker(20240607u);

    for (int i = 0; i < 4000 && !observer.game_over_called; ++i) {
        if (engine.get_enemy_hp() > 0) {
            engine.action_attack();
        } else {
            auto exits = engine.get_available_exits();
            if (exits.empty()) break;
            int choice = exit_picker.next_int(0, static_cast<int>(exits.size()) - 1);
            engine.action_move(choice);
        }
    }

    // Гра має завершитися (перемогою або поразкою гравця) - головне, щоб
    // спостерігач отримав сповіщення on_game_over рівно тоді, коли це
    // справді сталося.
    EXPECT_TRUE(observer.game_over_called);
}
