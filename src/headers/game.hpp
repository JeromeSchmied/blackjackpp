#pragma once

#include <string>

#include "dealer.hpp"
#include "deck.hpp"
#include "player.hpp"
#include "statistics.hpp"

class Game {
    // private:
   public:
    Player player;  // Player in the game (user)
    Dealer dealer;  // Dealer in the game
    Deck deck;      // Deck of cards in the game
    Statistics s;   // Leaderboard

    // public:
    Game();
    void deal1_dealer() { this->dealer.addCard(this->deal1()); }
    void deal1_player() { this->player.addCard(this->deal1()); }
    Card deal1() { return this->deck.deal(); }
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
