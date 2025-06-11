#pragma once

#include <string>

#include "dealer.hpp"
#include "deck.hpp"
#include "player.hpp"
#include "statistics.hpp"

class Game {
   private:
    Player player;  // Player in the game (user)
    Dealer dealer;  // Dealer in the game
    Deck deck;      // Deck of cards in the game
    Statistics s;   // Leaderboard

   public:
    Game();
    bool dealDealer();
    char compareSum();
    bool checkWins();
    char checkEnd();
    bool startBet();
    bool startGame();
    void beginGame();
    void beginMenu(const std::string& message = "");
    void saveGame();
    void loadGame();
    void printStatistics();
    static void printInstructions();
    void printTop();
    void printBody();
};
