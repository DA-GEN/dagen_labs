#include "GameController.h"

GameController::GameController(QObject* parent)
    : QObject(parent), engine_(std::make_unique<GameEngine>(*this)) {
}

int GameController::getPlayerHP() const { return engine_->get_player_hp(); }
int GameController::getPlayerMaxHP() const { return engine_->get_player_max_hp(); }
int GameController::getEnemyHP() const { return engine_->get_enemy_hp(); }

QVector<QString> GameController::getAvailableExits() const {
    QVector<QString> exits;
    for (const auto& exit : engine_->get_available_exits()) {
        exits.push_back(QString::fromStdString(exit));
    }
    return exits;
}

void GameController::startNewGame(QString playerName, int classChoice) {
    engine_->start_new_game(playerName.toStdString(), classChoice);
}

void GameController::actionMove(int exitIndex) { engine_->action_move(exitIndex); }
void GameController::actionAttack() { engine_->action_attack(); }
void GameController::actionTakeItem() { engine_->action_take_item(); }
void GameController::actionExitDungeon() { engine_->action_exit_dungeon(); }

// --- IGameObserver: переклад подій рушія у сигнали Qt ---

void GameController::on_log_message(const std::string& message) {
    emit logMessage(QString::fromStdString(message));
}

void GameController::on_stats_updated() {
    emit statsUpdated();
}

void GameController::on_room_updated(const std::string& description, bool has_enemy, bool has_item) {
    emit roomUpdated(QString::fromStdString(description), has_enemy, has_item);
}

void GameController::on_game_started() {
    emit gameStarted();
}

void GameController::on_game_over(bool victory) {
    emit gameOver(victory);
}
