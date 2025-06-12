#pragma once

#include <format>
#include <string>
class Card {
   private:
    int number;  // Card Number
    char suit;   // Card Suit
    bool block;  // Boolean value for Ace Switching

   public:
    // Default Constructor
    Card();
    // Parameterised Constructor (for initializing deck)
    Card(int no, char s);
    // Getter Functions
    int getNumber() const;

    char getSuit() const;
    bool getBlock() const;
    // Setter Functions
    void setNumber(int no);
    void setSuit(char c);
    void setBlock(bool b);
    // Printing Card Details
    char getPrintNumber() const;
    void printCardL1() const;
    void printCardL2() const;
    std::string dbg() const {
        return std::format("{{number: {}, suit: {}, block: {}, }}", this->number, this->suit, this->block);
    }
};
