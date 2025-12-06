#include "C:\Users\dagen\source\repos\dagen_labs\lab_1\include\Game.hpp"
#include <iostream>
#include <exception>

int main() {
    setlocale(LC_ALL, "Russian");
    
    try {
        Game game;
        game.initialize();
        game.game_loop();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cerr << "║              КРИТИЧНА ПОМИЛКА                         ║\n";
        std::cerr << "╚═══════════════════════════════════════════════════════╝\n";
        std::cerr << "\nВиникла неочікувана помилка:\n";
        std::cerr << e.what() << "\n\n";
        std::cerr << "Гру завершено.\n";
        std::cerr << "Будь ласка, повідомте про цю помилку, якщо вона повторюється.\n\n";
        return 1;
        
    } catch (...) {
        std::cerr << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cerr << "║              КРИТИЧНА ПОМИЛКА                         ║\n";
        std::cerr << "╚═══════════════════════════════════════════════════════╝\n";
        std::cerr << "\nВиникла невідома помилка!\n";
        std::cerr << "Гру завершено.\n\n";
        return 1;
    }
}


