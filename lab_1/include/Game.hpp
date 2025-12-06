#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include <string>
#include <vector>

// Підключаємо ваші існуючі класи
#include "GameMap.hpp"
#include "Player.hpp"
#include "Warrior.hpp"
#include "Mage.hpp"
#include "Archer.hpp"
#include "Enemy.hpp" // Переконайтеся, що цей файл підключено
#include "Item.hpp"

class Game : public QObject {
    Q_OBJECT

public:
    explicit Game(QObject* parent = nullptr)
        : QObject(parent), current_room_id_(0), game_running_(false), final_room_id_(0) {
    }

    // --- ГЕТТЕРИ ДЛЯ ІНТЕРФЕЙСУ (UI буде їх смикати, щоб оновити віджети) ---

    int getPlayerHP() const { return player_ ? player_->get_hp() : 0; }
    int getPlayerMaxHP() const { return player_ ? player_->get_max_hp() : 100; }

    // Повертає HP ворога в поточній кімнаті (або 0, якщо ворога немає)
    int getEnemyHP() const {
        if (dungeon_ && current_room_id_ >= 0) {
            auto* room = dungeon_->get_node_by_id(current_room_id_);
            if (room && room->has_enemy()) {
                return room->get_enemy()->get_hp();
            }
        }
        return 0;
    }

    // Повертає список назв сусідніх кімнат для кнопок навігації
    QVector<QString> getAvailableExits() const {
        QVector<QString> exits;
        if (!dungeon_) return exits;

        auto neighbors = dungeon_->get_neighbors(current_room_id_);
        for (auto* node : neighbors) {
            // Формуємо рядок типу "Кімната 2: Темний коридор"
            QString info = QString("Кімната %1: %2")
                .arg(node->get_id())
                .arg(QString::fromStdString(node->get_description()));
            exits.push_back(info);
        }
        return exits;
    }

signals:
    // --- СИГНАЛИ (Game -> UI) ---
    // UI має підписатися на ці сигнали, щоб знати, що показувати

    void logMessage(QString message);       // Додати текст у лог
    void statsUpdated();                    // Оновити HP бари та стати
    void roomUpdated(QString description, bool hasEnemy, bool hasItem); // Оновити опис кімнати та стан кнопок
    void gameStarted();                     // Гра почалася
    void gameOver(bool victory);            // Гра закінчилася

public slots:
    // --- СЛОТИ (UI -> Game) ---
    // Ці методи викликаються натисканням кнопок у вікні

    /**
     * @brief Початок нової гри
     * @param playerName Ім'я з поля вводу
     * @param classChoice Індекс з випадаючого списку (0-2)
     */
    void startNewGame(QString playerName, int classChoice) {
        std::string name = playerName.toStdString();
        if (name.empty()) name = "Герой";

        // Створення гравця
        switch (classChoice) {
        case 0: player_ = std::make_unique<Warrior>(name); break;
        case 1: player_ = std::make_unique<Mage>(name); break;
        case 2: player_ = std::make_unique<Archer>(name); break;
        default: player_ = std::make_unique<Warrior>(name);
        }

        generate_dungeon();
        current_room_id_ = 0;
        game_running_ = true;

        emit gameStarted();
        emit logMessage(QString("=== ЛАСКАВО ПРОСИМО, %1! ===").arg(playerName));
        emit logMessage("Ви увійшли у підземелля. Знайдіть вихід!");

        updateCurrentRoomInfo();
        emit statsUpdated();
    }

    /**
     * @brief Спроба переміщення в іншу кімнату
     * @param exitIndex Індекс кнопки, яку натиснув гравець (0, 1, 2...)
     */
    void actionMove(int exitIndex) {
        if (!game_running_) return;

        auto neighbors = dungeon_->get_neighbors(current_room_id_);

        if (exitIndex >= 0 && exitIndex < static_cast<int>(neighbors.size())) {
            current_room_id_ = neighbors[exitIndex]->get_id();
            emit logMessage(QString("\n---> Ви перейшли до кімнати %1").arg(current_room_id_));
            updateCurrentRoomInfo();
        }
        else {
            emit logMessage("Неможливо піти в цьому напрямку.");
        }
    }

    /**
     * @brief Виконання одного раунду бою
     */
    void actionAttack() {
        if (!game_running_ || !player_) return;

        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        if (!room || !room->has_enemy()) {
            emit logMessage("Тут немає кого атакувати.");
            return;
        }

        Enemy* enemy = room->get_enemy();

        // 1. Хід гравця
        // Примітка: Для кращого UI, ваші методи attack() в класах Player/Enemy 
        // мали б повертати string. Зараз ми просто логуємо факт удару.
        player_->attack(*enemy);
        emit logMessage(QString("Ви атакували %1!").arg(QString::fromStdString(enemy->get_name())));

        // 2. Перевірка смерті ворога
        if (!enemy->is_alive()) {
            emit logMessage(QString("🎉 ПЕРЕМОГА! %1 знищено.").arg(QString::fromStdString(enemy->get_name())));
            room->clear_enemy();
            updateCurrentRoomInfo(); // Оновити інтерфейс (прибрати кнопку атаки)
            emit statsUpdated();
            return;
        }

        // 3. Хід ворога
        enemy->attack(*player_);
        emit logMessage(QString("⚠️ %1 атакує вас у відповідь!").arg(QString::fromStdString(enemy->get_name())));

        // 4. Перевірка смерті гравця
        if (!player_->is_alive()) {
            emit logMessage("💀 ВАС ВБИТО! ГРА ЗАКІНЧЕНА.");
            game_running_ = false;
            emit gameOver(false);
        }

        emit statsUpdated();
    }

    /**
     * @brief Взаємодія з предметом у кімнаті
     */
    void actionTakeItem() {
        if (!game_running_) return;

        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        if (room && room->has_item()) {
            Item* item = room->get_item();
            player_->add_item(item); // Додаємо в інвентар
            emit logMessage(QString("Ви підібрали предмет: %1").arg(QString::fromStdString(item->get_name())));

            room->clear_item();
            updateCurrentRoomInfo();
            emit statsUpdated();
        }
    }

    /**
     * @brief Перевірка умови перемоги (вихід з підземелля)
     */
    void actionExitDungeon() {
        if (current_room_id_ == final_room_id_) {
            emit logMessage("🚪 ВИ ЗНАЙШЛИ ВИХІД! ПЕРЕМОГА!");
            game_running_ = false;
            emit gameOver(true);
        }
    }

private:
    std::unique_ptr<GameMap> dungeon_;
    std::unique_ptr<Player> player_;
    int current_room_id_;
    bool game_running_;
    int final_room_id_;

    // Допоміжний метод для генерації
    void generate_dungeon() {
        int num_rooms = 8 + (std::rand() % 5);
        int num_enemies = num_rooms / 2;
        int num_items = num_rooms / 2 + 1;

        dungeon_ = std::make_unique<GameMap>();
        dungeon_->generate_map(num_rooms, num_enemies, num_items);
        final_room_id_ = num_rooms - 1;
    }

    // Відправляє сигнали про стан поточної кімнати
    void updateCurrentRoomInfo() {
        if (!dungeon_) return;

        MapNode* room = dungeon_->get_node_by_id(current_room_id_);
        QString desc = QString::fromStdString(room->get_description());

        // Додаємо деталі до опису
        if (current_room_id_ == final_room_id_) {
            desc += "\n\n🚪 ТУТ Є ВИХІД З ПІДЗЕМЕЛЛЯ!";
        }
        if (room->has_enemy()) {
            desc += QString("\n\n👹 ТУТ ВОРОГ: %1 (HP: %2)")
                .arg(QString::fromStdString(room->get_enemy()->get_name()))
                .arg(room->get_enemy()->get_hp());
        }
        if (room->has_item()) {
            desc += QString("\n\n💎 ТУТ ПРЕДМЕТ: %1")
                .arg(QString::fromStdString(room->get_item()->get_name()));
        }

        emit roomUpdated(desc, room->has_enemy(), room->has_item());
    }
};

#endif // GAME_HPP