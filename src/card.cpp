#include "headers/card.h"

#include <array>
#include <iostream>

Card::Card() : number(0), suit('\0'), block(false) {}

Card::Card(int no, char s) : number(no), suit(s), block(false) {}

int Card::getNumber() const { return this->number; }

char Card::getSuit() const { return this->suit; }
bool Card::getBlock() const { return this->block; }

void Card::setNumber(int no) { this->number = no; }
void Card::setSuit(char c) { this->suit = c; }
void Card::setBlock(bool b) { this->block = b; }

char Card::getPrintNumber() const {
    switch (this->number) {
        case 1:
            return 'A';
        case 10:
            return 'X';
        case 11:
            return 'J';
        case 12:
            return 'Q';
        case 13:
            return 'K';
        default:
            std::array<char, 10> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
            return digits.at(this->number);
    }
}
void Card::printCardL1() const {
    switch (this->suit) {
        case 'C':
            std::cout << "| :(): |";
            break;
        case 'H':
            std::cout << "| (\\/) |";
            break;
        case 'D':
        case 'S':
            std::cout << "| :/\\: |";
            break;
        default:
            std::cout << "|  //  |";
    }
}

void Card::printCardL2() const {
    switch (this->suit) {
        case 'C':
            std::cout << "| ()() |";
            break;
        case 'H':
        case 'D':
            std::cout << "| :\\/: |";
            break;
        case 'S':
            std::cout << "| (__) |";
            break;
        default:
            std::cout << "|  //  |";
    }
}
