#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

#include "GameEngine.hpp"
#include "GameEvents.hpp"

// ЗМІНА (Архітектурний поділ, To-Be): GameController - єдине місце у
// застосунку, де ігрова логіка (core::GameEngine, без Qt) зустрічається з
// Qt. Він реалізує IGameObserver і транслює його виклики у звичні сигнали
// Qt, а також перекладає типи бібліотеки (std::string/std::vector) у
// Qt-типи (QString/QVector) для зручності GUI-шару.
//
// Завдяки цьому MainWindow і далі працює із сигналами/слотами так само, як
// раніше працював з класом Game - але вся ігрова логіка тепер живе в
// окремій, незалежній від Qt бібліотеці core і може повторно
// використовуватись чи тестуватися без QApplication.
class GameController : public QObject, private IGameObserver {
    Q_OBJECT

public:
    explicit GameController(QObject* parent = nullptr);

    int getPlayerHP() const;
    int getPlayerMaxHP() const;
    int getEnemyHP() const;
    QVector<QString> getAvailableExits() const;

signals:
    void logMessage(QString message);
    void statsUpdated();
    void roomUpdated(QString description, bool hasEnemy, bool hasItem);
    void gameStarted();
    void gameOver(bool victory);

public slots:
    void startNewGame(QString playerName, int classChoice);
    void actionMove(int exitIndex);
    void actionAttack();
    void actionTakeItem();
    void actionExitDungeon();

private:
    // --- IGameObserver: приймає події від GameEngine і ретранслює їх як сигнали Qt ---
    void on_log_message(const std::string& message) override;
    void on_stats_updated() override;
    void on_room_updated(const std::string& description, bool has_enemy, bool has_item) override;
    void on_game_started() override;
    void on_game_over(bool victory) override;

    std::unique_ptr<GameEngine> engine_;
};

#endif // GAME_CONTROLLER_H
