#pragma once

#include <string>

#include "human.hpp"

class Player : public Human {
   private:
    std::string name;  // Name of Player
    int cash, bet;     // Player's Cash, Player's Bet
    int wins, loses;   // Player's Stats (number of wins and loses)

   public:
    Player();
    std::string getName() const;
    int getBet() const;
    int getCash() const;
    int getWins() const;
    int getLoses() const;
    void setName(std::string nm);
    void setBet(int b);
    void addCash(int c);
    void incrementWins();
    void incrementLoses();
    void clearCards();
};
