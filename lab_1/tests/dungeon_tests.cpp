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
#include "Potion.hpp"
#include "Weapon.hpp"
#include "Armor.hpp"
#include "MapNode.hpp"
#include "Graph.hpp"
#include "GameMap.hpp"
#include <sstream>
#include <iostream>

class CoutRedirect {
    std::ostringstream oss;
    std::streambuf* old;
public:
    CoutRedirect() : oss(), old(std::cout.rdbuf(oss.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
    std::string getOutput() { return oss.str(); }
};

TEST(CharacterTest, WarriorInitialization) {
    Warrior warrior("TestWarrior");
    EXPECT_EQ(warrior.get_name(), "TestWarrior");
    EXPECT_EQ(warrior.get_hp(), 150);
    EXPECT_EQ(warrior.get_max_hp(), 150);
    EXPECT_EQ(warrior.get_attack_power(), 25);
    EXPECT_EQ(warrior.get_defense(), 12);
}

TEST(CharacterTest, MageInitialization) {
    Mage mage("TestMage");
    EXPECT_EQ(mage.get_name(), "TestMage");
    EXPECT_EQ(mage.get_hp(), 80);
    EXPECT_EQ(mage.get_max_hp(), 80);
    EXPECT_EQ(mage.get_attack_power(), 30);
    EXPECT_EQ(mage.get_defense(), 5);
}

TEST(CharacterTest, ArcherInitialization) {
    Archer archer("TestArcher");
    EXPECT_EQ(archer.get_name(), "TestArcher");
    EXPECT_EQ(archer.get_hp(), 100);
    EXPECT_EQ(archer.get_max_hp(), 100);
    EXPECT_EQ(archer.get_attack_power(), 20);
    EXPECT_EQ(archer.get_defense(), 8);
}

TEST(CharacterTest, TakeDamage) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.take_damage(30);
    EXPECT_EQ(warrior.get_hp(), 132);
}

TEST(CharacterTest, MinimumDamage) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.take_damage(5);
    EXPECT_EQ(warrior.get_hp(), 149);
}

TEST(CharacterTest, Healing) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.take_damage(50);
    int hp_after_damage = warrior.get_hp();
    warrior.heal(30);
    EXPECT_EQ(warrior.get_hp(), hp_after_damage + 30);
}

TEST(CharacterTest, HealingCap) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.take_damage(20);
    warrior.heal(100);
    EXPECT_EQ(warrior.get_hp(), warrior.get_max_hp());
}

TEST(CharacterTest, CharacterDeath) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.take_damage(200);
    EXPECT_FALSE(warrior.is_alive());
    EXPECT_EQ(warrior.get_hp(), 0);
}

TEST(CharacterTest, NegativeDamage) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    int initial_hp = warrior.get_hp();
    warrior.take_damage(-10);
    EXPECT_EQ(warrior.get_hp(), initial_hp);
}

TEST(CharacterTest, NegativeHealing) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    int initial_hp = warrior.get_hp();
    warrior.heal(-10);
    EXPECT_EQ(warrior.get_hp(), initial_hp);
}

TEST(EnemyTest, GoblinInitialization) {
    Goblin goblin;
    EXPECT_EQ(goblin.get_name(), "Goblin");
    EXPECT_EQ(goblin.get_hp(), 40);
    EXPECT_EQ(goblin.get_attack_power(), 10);
    EXPECT_EQ(goblin.get_defense(), 3);
}

TEST(EnemyTest, OrcInitialization) {
    Orc orc;
    EXPECT_EQ(orc.get_name(), "Orc");
    EXPECT_EQ(orc.get_hp(), 100);
    EXPECT_EQ(orc.get_attack_power(), 20);
    EXPECT_EQ(orc.get_defense(), 10);
}

TEST(EnemyTest, WraithInitialization) {
    Wraith wraith;
    EXPECT_EQ(wraith.get_name(), "Wraith");
    EXPECT_EQ(wraith.get_hp(), 60);
    EXPECT_EQ(wraith.get_attack_power(), 18);
    EXPECT_EQ(wraith.get_defense(), 5);
}

TEST(EnemyTest, WraithPhysicalResistance) {
    CoutRedirect redirect;
    Wraith wraith;
    int initial_hp = wraith.get_hp();
    wraith.take_damage(20);
    EXPECT_EQ(wraith.get_hp(), initial_hp - 5);
}

TEST(CombatTest, WarriorAttack) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    Goblin goblin;
    int initial_hp = goblin.get_hp();
    warrior.attack(goblin);
    EXPECT_EQ(goblin.get_hp(), initial_hp - 27);
}

TEST(CombatTest, MageAttackBypassesDefense) {
    CoutRedirect redirect;
    Mage mage("TestMage");
    Orc orc;
    int initial_hp = orc.get_hp();
    mage.attack(orc);
    EXPECT_EQ(orc.get_hp(), initial_hp - 30);
}

TEST(CombatTest, GoblinAttack) {
    CoutRedirect redirect;
    Goblin goblin;
    Warrior warrior("TestWarrior");
    int initial_hp = warrior.get_hp();
    goblin.attack(warrior);
    EXPECT_EQ(warrior.get_hp(), initial_hp - 1);
}

TEST(CombatTest, OrcAttackModifier) {
    CoutRedirect redirect;
    Orc orc;
    Mage mage("TestMage");
    int initial_hp = mage.get_hp();
    orc.attack(mage);
    EXPECT_EQ(mage.get_hp(), initial_hp - 17);
}

TEST(ItemTest, PotionHealing) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.take_damage(50);
    int hp_after_damage = warrior.get_hp();
    
    Potion potion("Health Potion", "Restores HP", 30);
    potion.use(&warrior);
    EXPECT_EQ(warrior.get_hp(), hp_after_damage + 30);
}

TEST(ItemTest, WeaponBoost) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    int initial_attack = warrior.get_attack_power();
    
    Weapon weapon("Sword", "A sharp blade", 20);
    weapon.use(&warrior);
    EXPECT_EQ(warrior.get_attack_power(), initial_attack + 10);
}

TEST(ItemTest, ArmorBoost) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    int initial_defense = warrior.get_defense();
    
    Armor armor("Shield", "Protective gear", 15);
    armor.use(&warrior);
    EXPECT_EQ(warrior.get_defense(), initial_defense + 15);
}

TEST(ItemTest, NullCharacterHandling) {
    CoutRedirect redirect;
    Potion potion("Health Potion", "Restores HP", 30);
    EXPECT_NO_THROW(potion.use(nullptr));
}

TEST(PlayerTest, AddItemToInventory) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    Potion potion("Health Potion", "Restores HP", 30);
    
    EXPECT_EQ(warrior.inventory_size(), 0);
    warrior.add_item(&potion);
    EXPECT_EQ(warrior.inventory_size(), 1);
}

TEST(PlayerTest, GetItemFromInventory) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    Potion potion("Health Potion", "Restores HP", 30);
    
    warrior.add_item(&potion);
    Item* retrieved = warrior.get_item(0);
    EXPECT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->get_name(), "Health Potion");
}

TEST(PlayerTest, RemoveItemFromInventory) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    Potion potion("Health Potion", "Restores HP", 30);
    
    warrior.add_item(&potion);
    EXPECT_EQ(warrior.inventory_size(), 1);
    warrior.remove_item(0);
    EXPECT_EQ(warrior.inventory_size(), 0);
}

TEST(PlayerTest, GetItemInvalidIndex) {
    Warrior warrior("TestWarrior");
    Item* retrieved = warrior.get_item(999);
    EXPECT_EQ(retrieved, nullptr);
}

TEST(PlayerTest, AddNullItem) {
    CoutRedirect redirect;
    Warrior warrior("TestWarrior");
    warrior.add_item(nullptr);
    EXPECT_EQ(warrior.inventory_size(), 0);
}

TEST(GraphTest, AddNodesAndEdges) {
    Graph<int> graph;
    EXPECT_TRUE(graph.add_node(1));
    EXPECT_TRUE(graph.add_node(2));
    EXPECT_TRUE(graph.add_node(3));
    EXPECT_EQ(graph.size(), 3);
    
    EXPECT_NO_THROW(graph.add_edge(1, 2));
    EXPECT_TRUE(graph.has_edge(1, 2));
}

TEST(GraphTest, BFSPathfinding) {
    Graph<int> graph;
    graph.add_node(1);
    graph.add_node(2);
    graph.add_node(3);
    graph.add_node(4);
    
    graph.add_undirected_edge(1, 2);
    graph.add_undirected_edge(2, 3);
    graph.add_undirected_edge(3, 4);
    
    auto path = graph.bfs(1, 4);
    EXPECT_EQ(path.size(), 4);
    EXPECT_EQ(path[0], 1);
    EXPECT_EQ(path[3], 4);
}

TEST(MapNodeTest, MapNodeBasics) {
    MapNode node(1, "Test Room");
    EXPECT_EQ(node.get_id(), 1);
    EXPECT_EQ(node.get_description(), "Test Room");
    EXPECT_FALSE(node.has_enemy());
    EXPECT_FALSE(node.has_item());
    
    Goblin goblin;
    node.set_enemy(&goblin);
    EXPECT_TRUE(node.has_enemy());
    
    node.clear_enemy();
    EXPECT_FALSE(node.has_enemy());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
