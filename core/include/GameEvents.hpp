#ifndef GAME_EVENTS_HPP
#define GAME_EVENTS_HPP

#include <string>

// Observer: контракт, через який GameEngine повідомляє зовнішній світ про
// події гри. Раніше ця роль належала сигналам QObject, через що вся ігрова
// логіка (Game) була змушена успадковувати QObject і не могла існувати чи
// тестуватися без Qt. Тепер рушій нічого не знає про конкретного підписника -
// це може бути Qt-адаптер (GameController), консольний вивід, тестовий
// шпигун (spy) тощо.
class IGameObserver {
public:
    virtual ~IGameObserver() = default;

    virtual void on_log_message(const std::string& message) = 0;
    virtual void on_stats_updated() = 0;
    virtual void on_room_updated(const std::string& description, bool has_enemy, bool has_item) = 0;
    virtual void on_game_started() = 0;
    virtual void on_game_over(bool victory) = 0;
};

#endif // GAME_EVENTS_HPP
