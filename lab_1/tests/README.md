# Dungeon Crawler Unit Tests

This test suite contains 30 comprehensive unit tests using Google Test framework covering all major components of the dungeon crawler game.

## Test Coverage

### Character Tests (10 tests)
1. **WarriorInitialization** - Validates Warrior class initialization with correct stats
2. **MageInitialization** - Validates Mage class initialization with correct stats
3. **ArcherInitialization** - Validates Archer class initialization with correct stats
4. **TakeDamage** - Tests damage calculation with defense reduction
5. **MinimumDamage** - Tests minimum damage rule (at least 1 damage always gets through)
6. **Healing** - Tests character healing mechanics
7. **HealingCap** - Tests that healing cannot exceed max HP
8. **CharacterDeath** - Tests character death when HP reaches 0
9. **NegativeDamage** - Tests that negative damage values are ignored
10. **NegativeHealing** - Tests that negative healing values are ignored

### Enemy Tests (4 tests)
11. **GoblinInitialization** - Validates Goblin enemy stats
12. **OrcInitialization** - Validates Orc enemy stats
13. **WraithInitialization** - Validates Wraith enemy stats
14. **WraithPhysicalResistance** - Tests Wraith's 50% physical damage resistance

### Combat Tests (4 tests)
15. **WarriorAttack** - Tests Warrior's 120% damage modifier
16. **MageAttackBypassesDefense** - Tests Mage's defense-ignoring magic attacks
17. **GoblinAttack** - Tests basic enemy attack mechanics
18. **OrcAttackModifier** - Tests Orc's 110% damage modifier

### Item Tests (4 tests)
19. **PotionHealing** - Tests potion healing functionality
20. **WeaponBoost** - Tests weapon attack power boost
21. **ArmorBoost** - Tests armor defense boost
22. **NullCharacterHandling** - Tests safe handling of null character pointers

### Player Inventory Tests (5 tests)
23. **AddItemToInventory** - Tests adding items to player inventory
24. **GetItemFromInventory** - Tests retrieving items from inventory
25. **RemoveItemFromInventory** - Tests removing items from inventory
26. **GetItemInvalidIndex** - Tests safe handling of invalid inventory indices
27. **AddNullItem** - Tests safe handling of null item pointers

### Graph Tests (2 tests)
28. **AddNodesAndEdges** - Tests graph node and edge creation
29. **BFSPathfinding** - Tests breadth-first search pathfinding algorithm

### git checkout -b feature/docs-cigit checkout -b feature/docs-cigit checkout -b feature/docs-cigit checkout -b 30. **MapNodeBasics** - Tests map node functionality including enemy and item management

## Building and Running Tests

### Build the tests:
```bash
cmake -S . -B build
cmake --build build --target dungeon_tests
```

### Run the tests:
```bash
./build/dungeon_tests
```

### Run with verbose output:
```bash
./build/dungeon_tests --gtest_verbose
```

### Run specific test:
```bash
./build/dungeon_tests --gtest_filter=CharacterTest.WarriorInitialization
```

## Test Design

The tests use a `CoutRedirect` helper class to suppress console output during testing, ensuring clean test output. Each test is isolated and independent, testing specific functionality without side effects.

The test suite covers:
- **Positive scenarios**: Normal game mechanics and expected behavior
- **Negative scenarios**: Edge cases like negative values, null pointers, invalid indices
- **Boundary conditions**: Max HP caps, minimum damage rules, death conditions
- **Special mechanics**: Class-specific abilities (Mage magic, Warrior power, Wraith resistance)

## Dependencies

- Google Test (automatically fetched via CMake FetchContent)
- C++17 compiler
- CMake 3.10 or higher

## Documentation

Documentation is generated automatically using Doxygen and GitHub Actions.
You can view the latest API reference here: https://da-gen.github.io/dagen_labs/