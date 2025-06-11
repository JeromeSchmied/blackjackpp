#include "headers/human.hpp"

#include <iostream>

// Default Constructor
Human::Human() : sum(0) {}

// Getter Function for sum to check end of game
int Human::getSum() {
    switchAce();
    return this->sum;
}

// Switches Ace between 1 and 11
void Human::switchAce() {
    if (this->sum > 21) {
        for (auto& i : this->hand) {
            if (i.getNumber() == 1 && !(i.getBlock())) {
                i.setBlock(true);
                this->sum -= 10;
                return;
            }
        }
    }
}

// Adds card to Human's hand
void Human::addCard(Card c) {
    this->hand.push_back(c);
    if (c.getNumber() > 10) {
        c.setNumber(10);
    } else if (c.getNumber() == 1) {
        c.setNumber(11);
    }
    this->sum += c.getNumber();
}

// Clears Human's hand
void Human::clearCards() {
    this->hand.clear();
    this->sum = 0;
}

// Prints Human's cards
void Human::printCards() {
    std::cout << "\n";
    for (int i = 0; i < 6; i++) {
        for (auto& j : this->hand) {
            switch (i) {
                case 0:
                    std::cout << ".------.";
                    break;
                case 1:
                    std::cout << "|" << j.getPrintNumber() << ".--. |";
                    break;
                case 2:
                    j.printCardL1();
                    break;
                case 3:
                    j.printCardL2();
                    break;
                case 4:
                    std::cout << "| '--'" << j.getPrintNumber() << "|";
                    break;
                case 5:
                    std::cout << "`------'";
                default:
                    break;
            }
        }
        std::cout << "\n";
    }
}
