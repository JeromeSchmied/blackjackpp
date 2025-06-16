#include "headers/deck.hpp"

#include <algorithm>
#include <array>

#include "headers/random.hpp"

// Constructs a Deck
void Deck::initializeDeck() {
    this->deck.clear();
    std::array<char, 4> suits = {'S', 'H', 'D', 'C'};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            Card c(j + 1, suits.at(i));
            this->deck.push_back(c);
        }
    }

    std::shuffle(this->deck.begin(), this->deck.end(), rng::custom_random);
}

// Getter Function for size of deck
int Deck::getSize() { return static_cast<int>(this->deck.size()); }

// Deals by returning one card from the deck
Card Deck::deal() {
    Card t = this->deck.back();
    this->deck.pop_back();
    return t;
}
