#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "GameMap.hpp"
#include "GameEvents.hpp"
#include "RandomProvider.hpp"
#include "EnemyFactory.hpp"
#include "ItemFactory.hpp"
#include "Player.hpp"
#include "Warrior.hpp"
#include "Mage.hpp"
#include "Archer.hpp"
#include "Enemy.hpp"
#include "Item.hpp"

#include <memory>
#include <string>
#include <vector>

// ЗМІНА: GameEngine - це "Архітектурний поділ" (To-Be) колишнього класу
// Game. Уся ігрова логіка перенесена сюди в чистому, незалежному від Qt
// вигляді: жодних QObject/QString/сигналів. Замість сигналів рушій повідомляє
// підписника через інтерфейс IGameObserver (патерн Спостерігач). Завдяки
// цьому:
//   - логіку можна юніт-тестувати без створення Qt-застосунку (QApplication);
//   - бібліотеку core можна повторно використати в іншому інтерфейсі
//     (консоль, інша GUI-бібліотека, мережевий клієнт-сервер тощо).
// Класи зі спеціалізованим Qt-кодом (GameController/MainWindow) лише
// підписуються на події цього рушія й транслюють їх у відповідний UI-шар.
class GameEngine {
public:
    explicit GameEngine(IGameObserver& observer)
        : observer_(observer),
        rng_(),
        enemy_factory_(rng_),
        item_factory_(rng_),
        current_room_id_(0),
        game_running_(false),
        final_room_id_(0) {
    }

    // --- ЗАПИТИ СТАНУ (UI смикає їх, щоб оновити віджети) ---

    int get_player_hp() const { return player_ ? player_->get_hp() : 0; }
    int get_player_max_hp() const { return player_ ? player_->get_max_hp() : 100; }

    int get_enemy_hp() const {
        if (dungeon_ && current_room_id_ >= 0) {
            auto* room = dungeon_->get_node_by_id(current_room_id_);
            if (room && room->has_enemy()) {
                return room->get_enemy()->get_hp();
            }
        }
        return 0;
    }

    // Повертає описи сусідніх кімнат для кнопок навігації
    std::vector<std::string> get_available_exits() const {
        std::vector<std::string> exits;
        if (!dungeon_) return exits;

        auto neighbors = dungeon_->get_neighbors(current_room_id_);
        for (auto* node : neighbors) {
            exits.push_back("Кімната " + std::to_string(node->get_id()) + ": " + node->get_description());
        }
        return exits;
    }

    // --- ДІЇ (UI -> GameEngine) ---

    void start_new_game(const std::string& player_name, int class_choice) {
        std::string name = player_name.empty() ? "Герой" : player_name;

        switch (class_choice) {
        case 0: player_ = std::make_unique<Warrior>(name); break;
        case 1: player_ = std::make_unique<Mage>(name); break;
        case 2: player_ = std::make_unique<Archer>(name, rng_); break;
        default: player_ = std::make_unique<Warrior>(name);
        }

        generate_dungeon();
        current_room_id_ = 0;
        game_running_ = true;

        observer_.on_game_started();
        // ЗМІНА: тут було пряме використання вхідного player_name, через що
        // привітання показувало порожній рядок для гравця без імені -
        // підсумкове ім'я (з фолбеком "Герой") тепер обчислюється один раз
        // у змінній name і використовується послідовно.
        observer_.on_log_message("=== ЛАСКАВО ПРОСИМО, " + name + "! ===");
        observer_.on_log_message("Ви увійшли у підземелля. Знайдіть вихід!");

        notify_current_room();
        observer_.on_stats_updated();
    }

    void action_move(int exit_index) {
        if (!game_running_) return;

        MapNode* current_room = dungeon_->get_node_by_id(current_room_id_);
        if (current_room->has_enemy()) {
            observer_.on_log_message("⛔ Ви не можете вийти з кімнати під час бою! Переможіть ворога.");
            return;
        }

        auto neighbors = dungeon_->get_neighbors(current_room_id_);

        if (exit_index >= 0 && exit_index < static_cast<int>(neighbors.size())) {
            current_room_id_ = neighbors[static_cast<size_t>(exit_index)]->get_id();
            observer_.on_log_message("\n---> Ви перейшли до кімнати " + std::to_string(current_room_id_));
            notify_current_room();
            observer_.on_stats_updated();
        } else {
            observer_.on_log_message("Неможливо піти в цьому напрямку.");
        }
    }

    void action_attack() {
        if (!game_running_ || !player_) return;

        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        if (!room || !room->has_enemy()) {
            observer_.on_log_message("Тут немає кого атакувати.");
            return;
        }

        Enemy* enemy = room->get_enemy();

        // 1. Хід гравця
        player_->attack(*enemy);
        observer_.on_log_message("Ви атакували " + enemy->get_name() + "!");

        // 2. Перевірка смерті ворога
        if (!enemy->is_alive()) {
            observer_.on_log_message("🎉 ПЕРЕМОГА! " + enemy->get_name() + " знищено.");
            room->clear_enemy();

            if (dungeon_->allEnemiesDefeated()) {
                observer_.on_log_message("\n🏆 ВІТАЄМО! ПІДЗЕМЕЛЛЯ ЗАЧИЩЕНО!");
                observer_.on_log_message("Всі вороги знищені. Ви справжній герой!");
                game_running_ = false;
                observer_.on_game_over(true);
            } else {
                observer_.on_log_message("Підземелля стало трохи безпечнішим, але вороги ще залишилися...");
            }

            notify_current_room();
            observer_.on_stats_updated();
            return;
        }

        // 3. Хід ворога
        enemy->attack(*player_);
        observer_.on_log_message("⚠️ " + enemy->get_name() + " атакує вас у відповідь!");

        // 4. Перевірка смерті гравця
        if (!player_->is_alive()) {
            observer_.on_log_message("💀 ВАС ВБИТО! ГРА ЗАКІНЧЕНА.");
            game_running_ = false;
            observer_.on_game_over(false);
        }

        observer_.on_stats_updated();
    }

    void action_take_item() {
        if (!game_running_) return;

        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        if (room && room->has_item()) {
            Item* item = room->get_item();
            player_->add_item(item);
            observer_.on_log_message("Ви підібрали предмет: " + item->get_name());

            room->clear_item();
            notify_current_room();
            observer_.on_stats_updated();
        }
    }

    void action_exit_dungeon() {
        if (current_room_id_ == final_room_id_) {
            observer_.on_log_message("🚪 ВИ ЗНАЙШЛИ ВИХІД! ПЕРЕМОГА!");
            game_running_ = false;
            observer_.on_game_over(true);
        }
    }

private:
    IGameObserver& observer_;
    RandomProvider rng_;
    EnemyFactory enemy_factory_;
    ItemFactory item_factory_;

    std::unique_ptr<GameMap> dungeon_;
    std::unique_ptr<Player> player_;
    int current_room_id_;
    bool game_running_;
    int final_room_id_;

    void generate_dungeon() {
        int num_rooms = 8 + rng_.next_int(0, 4);
        int num_enemies = num_rooms / 2;
        int num_items = num_rooms / 2 + 1;

        dungeon_ = std::make_unique<GameMap>(rng_, enemy_factory_, item_factory_);
        dungeon_->generate_map(num_rooms, num_enemies, num_items);
        final_room_id_ = num_rooms - 1;
    }

    void notify_current_room() {
        if (!dungeon_) return;

        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        std::string desc = room->get_description();

        if (current_room_id_ == final_room_id_) {
            desc += "\n\n🚪 ТУТ Є ВИХІД З ПІДЗЕМЕЛЛЯ!";
        }
        if (room->has_enemy()) {
            desc += "\n\n👹 ТУТ ВОРОГ: " + room->get_enemy()->get_name() +
                " (HP: " + std::to_string(room->get_enemy()->get_hp()) + ")";
        }
        if (room->has_item()) {
            desc += "\n\n💎 ТУТ ПРЕДМЕТ: " + room->get_item()->get_name();
        }

        observer_.on_room_updated(desc, room->has_enemy(), room->has_item());
    }
};

#endif // GAME_ENGINE_HPP
