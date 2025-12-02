#ifndef GAME_HPP
#define GAME_HPP

#include "GameMap.hpp"
#include "Player.hpp"
#include "Warrior.hpp"
#include "Mage.hpp"
#include "Archer.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <limits>

/**
 * @brief Main game controller managing game loop and state
 */
class Game {
private:
    std::unique_ptr<GameMap> dungeon_;   ///< Dungeon map
    std::unique_ptr<Player> player_;     ///< Player character
    int current_room_id_;                ///< Current room ID
    bool game_running_;                  ///< Game running flag
    bool game_won_;                      ///< Victory flag
    int final_room_id_;                  ///< Exit room ID

    /** @brief Clear input buffer */
    void clear_input() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * @brief Get validated integer input
     * @param prompt Input prompt
     * @return Integer value
     */
    int get_int_input(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clear_input();
                return value;
            }
            std::cout << "Невірне введення! Введіть число." << std::endl;
            clear_input();
        }
    }

    /** @brief Display welcome screen */
    void display_welcome() {
        std::cout << "\n";
        std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
        std::cout << "║                                                           ║\n";
        std::cout << "║         ЛАСКАВО ПРОСИМО ДО ГРИ DUNGEON CRAWLER!           ║\n";
        std::cout << "║                                                           ║\n";
        std::cout << "║   Досліджуйте процедурно згенероване підземелля,          ║\n";
        std::cout << "║   перемагайте ворогів, збирайте предмети та знайдіть вихід! ║\n";
        std::cout << "║                                                           ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
        std::cout << std::endl;
    }

    /** @brief Create player character with class selection */
    void create_player() {
        std::cout << "Оберіть клас персонажа:\n";
        std::cout << "1. Воїн (Високе HP, Сильний захист)\n";
        std::cout << "2. Маг (Магія ігнорує захист)\n";
        std::cout << "3. Лучник (Шанс критичного удару)\n";
        
        int choice = get_int_input("Ваш вибір (1-3): ");
        
        std::cout << "\nВведіть ім'я вашого персонажа: ";
        std::string name;
        std::getline(std::cin, name);
        
        if (name.empty()) {
            name = "Шукач пригод";
        }
        
        switch (choice) {
            case 1:
                player_ = std::make_unique<Warrior>(name);
                break;
            case 2:
                player_ = std::make_unique<Mage>(name);
                break;
            case 3:
                player_ = std::make_unique<Archer>(name);
                break;
            default:
                std::cout << "Невірний вибір! Створюємо Воїна за замовчуванням.\n";
                player_ = std::make_unique<Warrior>(name);
        }
        
        std::cout << "\nПерсонажа створено!\n";
        player_->display_stats();
    }

    /** @brief Generate procedural dungeon */
    void generate_dungeon() {
        std::cout << "\nГенерація підземелля...\n";
        
        int num_rooms = 8 + (std::rand() % 5); // 8-12 rooms
        int num_enemies = num_rooms / 2;
        int num_items = num_rooms / 2 + 1;
        
        dungeon_ = std::make_unique<GameMap>();
        dungeon_->generate_map(num_rooms, num_enemies, num_items);
        
        final_room_id_ = num_rooms - 1;
        
        std::cout << "Підземелля згенероване: " << num_rooms << " кімнат, "
                  << num_enemies << " ворогів, " << num_items << " предметів\n";
    }

    /** @brief Display current room information */
    void display_current_room() {
        std::cout << "\n" << std::string(60, '-') << "\n";
        dungeon_->display_room_details(current_room_id_);
        
        if (current_room_id_ == final_room_id_) {
            std::cout << "🚪 *** ЦЕ ВИХІД! ***\n";
        }
    }

    /** @brief Handle combat encounter */
    void handle_combat() {
        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        if (!room || !room->has_enemy()) {
            return;
        }
        
        Enemy* enemy = room->get_enemy();
        std::cout << "\n⚔️  ПОЧАТОК БОЮ!\n\n";
        
        // Combat loop
        int round = 1;
        while (player_->is_alive() && enemy->is_alive()) {
            std::cout << ">>> Раунд " << round << " <<<\n";
            
            // Player attacks
            player_->attack(*enemy);
            
            if (!enemy->is_alive()) {
                std::cout << "\n🎉 Перемога! Ворога переможено!\n";
                room->clear_enemy();
                break;
            }
            
            // Enemy attacks
            enemy->attack(*player_);
            
            if (!player_->is_alive()) {
                std::cout << "\n💀 Вас переможено...\n";
                game_running_ = false;
                break;
            }
            
            std::cout << "\nНатисніть Enter для продовження...";
            std::cin.get();
            
            round++;
        }
    }

    /** @brief Handle item discovery */
    void handle_item() {
        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        if (!room || !room->has_item()) {
            return;
        }
        
        Item* item = room->get_item();
        std::cout << "\n💎 Знайдено предмет: " << item->get_name() << "\n";
        std::cout << "Опис: " << item->get_description() << "\n";
        
        std::cout << "\nХочете використати його? (y/n): ";
        char choice;
        std::cin >> choice;
        clear_input();
        
        if (choice == 'y' || choice == 'Y') {
            item->use(player_.get());
            room->clear_item();
            std::cout << "\n";
            player_->display_stats();
        } else {
            // Add to inventory
            player_->add_item(item);
            room->clear_item();
        }
    }

    /** @brief Display available player actions */
    void display_actions() {
        std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cout << "║                    ДОСТУПНІ ДІЇ                       ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════╝\n";
        std::cout << "1. Перейти до іншої кімнати\n";
        std::cout << "2. Переглянути характеристики персонажа\n";
        std::cout << "3. Переглянути інвентар\n";
        std::cout << "4. Використати предмет з інвентаря\n";
        
        if (current_room_id_ == final_room_id_) {
            std::cout << "5. ВИЙТИ З ПІДЗЕМЕЛЛЯ (ПЕРЕМОГА!)\n";
        }
        
        std::cout << "0. Вийти з гри\n";
    }

    /** @brief Handle room movement */
    void move_to_room() {
        auto neighbors = dungeon_->get_neighbors(current_room_id_);
        
        if (neighbors.empty()) {
            std::cout << "Немає виходів з цієї кімнати!\n";
            return;
        }
        
        std::cout << "\nДоступні виходи:\n";
        for (size_t i = 0; i < neighbors.size(); ++i) {
            std::cout << (i + 1) << ". Кімната " << neighbors[i]->get_id() 
                      << " - " << neighbors[i]->get_description() << "\n";
        }
        
        int choice = get_int_input("\nОберіть кімнату (1-" + std::to_string(neighbors.size()) + "): ");
        
        if (choice >= 1 && choice <= static_cast<int>(neighbors.size())) {
            current_room_id_ = neighbors[choice - 1]->get_id();
            std::cout << "\nВи переходите до Кімнати " << current_room_id_ << "...\n";
        } else {
            std::cout << "Невірний вибір!\n";
        }
    }

    /** @brief Use item from inventory */
    void use_inventory_item() {
        if (player_->inventory_size() == 0) {
            std::cout << "Ваш інвентар порожній!\n";
            return;
        }
        
        player_->show_inventory();
        
        int choice = get_int_input("\nОберіть предмет для використання (1-" + 
                                  std::to_string(player_->inventory_size()) + ", 0 для скасування): ");
        
        if (choice >= 1 && choice <= static_cast<int>(player_->inventory_size())) {
            Item* item = player_->get_item(choice - 1);
            if (item) {
                item->use(player_.get());
                player_->remove_item(choice - 1);
                std::cout << "\n";
                player_->display_stats();
            }
        }
    }

    /** @brief Process player action input */
    void process_action() {
        display_actions();
        
        int choice = get_int_input("\nВаш вибір: ");
        
        switch (choice) {
            case 1:
                move_to_room();
                break;
            case 2:
                std::cout << "\n";
                player_->display_stats();
                break;
            case 3:
                player_->show_inventory();
                break;
            case 4:
                use_inventory_item();
                break;
            case 5:
                if (current_room_id_ == final_room_id_) {
                    game_won_ = true;
                    game_running_ = false;
                } else {
                    std::cout << "Невірний вибір!\n";
                }
                break;
            case 0:
                std::cout << "\nВи впевнені, що хочете вийти? (y/n): ";
                char confirm;
                std::cin >> confirm;
                clear_input();
                if (confirm == 'y' || confirm == 'Y') {
                    game_running_ = false;
                }
                break;
            default:
                std::cout << "Невірний вибір!\n";
        }
    }

public:
    /**
     * @brief Construct a new Game
     */
    Game() : current_room_id_(0), game_running_(false), game_won_(false), final_room_id_(0) {}

    /**
     * @brief Initialize game (player creation, dungeon generation)
     */
    void initialize() {
        try {
            display_welcome();
            create_player();
            generate_dungeon();
            
            current_room_id_ = 0;
            game_running_ = true;
            game_won_ = false;
            
            std::cout << "\nГру успішно ініціалізовано!\n";
            std::cout << "Натисніть Enter, щоб розпочати пригоду...";
            std::cin.get();
            
        } catch (const std::exception& e) {
            std::cerr << "Помилка під час ініціалізації: " << e.what() << std::endl;
            throw;
        }
    }

    /**
     * @brief Main game loop
     */
    void game_loop() {
        try {
            while (game_running_ && player_->is_alive()) {
                display_current_room();
                
                if (dungeon_->get_node_by_id(current_room_id_)->has_enemy()) {
                    handle_combat();
                    if (!player_->is_alive()) {
                        break;
                    }
                }
                
                if (dungeon_->get_node_by_id(current_room_id_)->has_item()) {
                    handle_item();
                }
                
                if (!game_running_) {
                    break;
                }
                
                process_action();
            }
            
            display_game_over();
            
        } catch (const std::exception& e) {
            std::cerr << "Помилка під час ігрового циклу: " << e.what() << std::endl;
            throw;
        }
    }

    /**
     * @brief Display game over screen with results
     */
    void display_game_over() {
        std::cout << "\n\n";
        std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
        std::cout << "║                                                           ║\n";
        
        if (game_won_) {
            std::cout << "║                   🎉 ПЕРЕМОГА! 🎉                         ║\n";
            std::cout << "║                                                           ║\n";
            std::cout << "║        Ви успішно вибралися з підземелля!                 ║\n";
        } else if (!player_->is_alive()) {
            std::cout << "║                   💀 ПОРАЗКА 💀                           ║\n";
            std::cout << "║                                                           ║\n";
            std::cout << "║          Ви загинули в підземеллі...                      ║\n";
        } else {
            std::cout << "║                   КІНЕЦЬ ГРИ                              ║\n";
            std::cout << "║                                                           ║\n";
            std::cout << "║               Дякуємо за гру!                             ║\n";
        }
        
        std::cout << "║                                                           ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
        
        std::cout << "\n=== Фінальна статистика ===\n";
        player_->display_stats();
        
        std::cout << "\nДякуємо за гру в Dungeon Crawler!\n";
        std::cout << "Створено на C++17 з використанням систем Graph, Character, Item та Map.\n\n";
    }
};

#endif // GAME_HPP

