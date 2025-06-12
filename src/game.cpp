#include "headers/game.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "headers/print.hpp"
#include "print.hpp"
#include "tui.hpp"

namespace color = tui::text::color;
namespace style = tui::text::style;

//////////////* Default Constructor *////

Game::Game() { this->deck.initializeDeck(); }

//////////////* Deals dealer towards the end *////

bool Game::dealDealer() {
    if (this->dealer.getSum() < this->player.getSum()) {
        while (this->dealer.getSum() < 17) {
            this->dealer.addCard(this->deck.deal());
            if (checkWins()) {
                return false;
            }
        }
        return true;
    }
    if (checkWins()) {
        return false;
    }
    return true;
}

//////////////* Checkers *////

char Game::compareSum() {
    if (this->player.getSum() > this->dealer.getSum()) {
        // printTop();
        // std::cout << tui::string(Print::you_win()).yellow() << style::reset_style() << "\n    (Dealer has
        // "
        // << this->dealer.getSum() << ")\n";
        return 'p';
    }
    if (this->dealer.getSum() > this->player.getSum()) {
        // printTop();
        // std::cout << tui::string(Print::dealer_wins()).red() << "\n    (" << this->dealer.getSum() <<
        // ")\n";
        return 'd';
    }
    // printTop();
    // std::cout << tui::string(Print::draw()).magenta();
    return 'n';
}

bool Game::checkWins() {
    switch (checkEnd()) {
        case 'f':
            return false;
        case 'd':
            this->player.incrementLoses();
            return true;
        case 'p':
            this->player.incrementWins();
            this->player.addCash((this->player.getBet() * 2));
            return true;
        default:
            return false;
    }
}

char Game::checkEnd() {
    if (this->dealer.getSum() > 21 || this->player.getSum() > 21) {
        // printTop();
        // std::cout << tui::string(Print::bust()).red() << "\n    [Dealer : " << this->dealer.getSum()
        // << " | " << this->player.getName() << " : " << this->player.getSum() << "]\n";
        if (this->dealer.getSum() > 21) {
            return 'p';
        }
        if (this->player.getSum() > 21) {
            return 'd';
        }
    } else if (this->dealer.getSum() == 21 || this->player.getSum() == 21) {
        // printTop();
        // std::cout << tui::string(Print::blackjack()).green() << "\n    [Dealer : " <<
        // this->dealer.getSum()
        // << " | " << this->player.getName() << " : " << this->player.getSum() << "]\n";
        if (this->dealer.getSum() == 21) {
            return 'd';
        }
        if (this->player.getSum() == 21) {
            return 'p';
        }
    }
    return 'f';
}

//////////////* Game Starters *////

bool Game::startBet() {
    if (this->player.getCash() > 0) {
        while (true) {
            printTop();
            std::cout << "Place your bet!\t\t $" << tui::string(this->player.getBet()).green()
                      << "\n[W = Raise Bet | S = Decrease Bet | R = Done]\n";
            switch (toupper(read_ch())) {
                case 'W':
                    if (this->player.getCash() >= 5) {
                        this->player.setBet(5);
                    }
                    break;
                case 'S':
                    if (this->player.getBet() >= 5) {
                        this->player.setBet(-5);
                    }
                    break;
                case 'R':
                case '\n':
                    return true;
                default:
                    break;
            }
        }
        return true;
    }
    return false;
}

bool Game::startGame() {
    this->player.addCard(this->deck.deal());
    this->dealer.addCard(this->deck.deal());
    this->player.addCard(this->deck.deal());
    this->dealer.addCard(this->deck.deal());
    printBody();
    if (checkWins()) {
        return false;
    }
    while (true) {
        std::cout << tui::string("\n\nH : Hit | S : Stand\n").yellow();
        int c = toupper(read_ch());
        if (c == 72) {
            this->player.addCard(this->deck.deal());
            printBody();
            if (checkWins()) {
                return false;
            }
        } else if (c == 83) {
            break;
        }
    }
    return true;
}

void Game::beginGame() {
    char cont = 0;
    do {
        if (this->deck.getSize() < 36) {
            this->deck.initializeDeck();
        }
        this->player.clearCards();
        this->dealer.clearCards();
        if (!startBet()) {
            std::cout << tui::string("\nBankrupt! Game over.\n").yellow();
            break;
        }
        if (startGame()) {
            if (dealDealer()) {
                switch (compareSum()) {
                    case 'p':
                        this->player.incrementWins();
                        this->player.addCash((this->player.getBet() * 2));
                        break;
                    case 'd':
                        this->player.incrementLoses();
                        break;
                    case 'n':
                        this->player.addCash(this->player.getBet());
                        break;
                    default:
                        break;
                }
            }
        }
        std::cout << tui::string(Print::dealer_border()).red();
        this->dealer.printCards();
        std::cout << tui::string(Print::player_border()).cyan();
        this->player.printCards();
        std::cout << color::yellow_fg() << "\nYour wins: " << this->player.getWins() << color::red_fg()
                  << "\nYour loses: " << this->player.getLoses() << style::reset_style() << "\n";
        if (s.check(this->player)) {
            std::cout << tui::string("High Score!\n").yellow();
        }
        std::cout << "\nContinue playing? [Y/N]: ";
        std::cin >> cont;
    } while (cont != 'N' && cont != 'n');
    char saveChoice = 0;
    std::cout << "\nSave game? [Y/N]: ";
    std::cin >> saveChoice;
    if (saveChoice == 'Y' || saveChoice == 'y') {
        saveGame();
    }
}

//////////////* Main Method to be Called *////

void Game::beginMenu(const std::string &message) {
    tui::screen::clear();
    tui::cursor::home();
    std::cout << tui::string(Print::title_blackjack()).yellow() << "\n";
    std::cout << Print::begin_menu() << "\n";
    if (!message.empty()) {
        std::cout << tui::string(message).red() << "\n";
    }
    char c = 0;
    std::cout << "Input : ";
    std::cin >> c;
    std::string nm;
    switch (c) {
        case '1':
            std::cout << "Enter player name: ";
            std::cin >> nm;
            this->player.setName(nm);
            beginGame();
            break;
        case '2':
            loadGame();
            beginGame();
            break;
        case '3':
            printStatistics();
            beginMenu();
            break;
        case '4':
            printInstructions();
            beginMenu();
            break;
        case '5':
            exit(0);
            break;
        default:
            beginMenu("Invalid input.");
    }
}

//////////////* Data File Handling *////

void Game::saveGame() {
    std::fstream f1;
    std::fstream f2;
    std::string filename;
    std::string path = "data/";
    do {
        std::cout << "Enter filename: ";
        std::cin >> filename;
        std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    } while (filename == "statistics");
    path += filename + ".bin";
    std::string sName = this->player.getName();
    int sCash = this->player.getCash();
    int sWins = this->player.getWins();
    int sLoses = this->player.getLoses();
    int nameSize = static_cast<int>(sName.size());
    f2.open(path, std::ios::in | std::ios::binary);
    if (!f2.fail()) {
        char choice = 0;
        std::cout << tui::string("File already exists.").red() << " Do you want to overwrite it? [Y/N]: ";
        std::cin >> choice;
        if (choice == 'N' || choice == 'n') {
            saveGame();
        }
    }
    f2.close();
    f1.open(path, std::ios::out | std::ios::binary);
    f1.write(reinterpret_cast<char *>(&nameSize), sizeof(nameSize));
    f1.write(sName.c_str(), static_cast<int64_t>(sName.size()));
    f1.write(reinterpret_cast<char *>(&sCash), sizeof(sCash));
    f1.write(reinterpret_cast<char *>(&sWins), sizeof(sWins));
    f1.write(reinterpret_cast<char *>(&sLoses), sizeof(sLoses));
    f1.close();
}

void Game::loadGame() {
    std::fstream f1;
    std::string filename;
    std::string path = "data/";
    do {
        std::cout << "Enter filename: ";
        std::cin >> filename;
        std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    } while (filename == "statistics");
    path += filename + ".bin";
    f1.open(path, std::ios::in | std::ios::binary);
    if (!f1.fail()) {
        std::string sName;
        int sCash = 0;
        int sWins = 0;
        int sLoses = 0;
        int nameSize = 0;
        f1.read(reinterpret_cast<char *>(&nameSize), sizeof(nameSize));
        sName.resize(nameSize);
        f1.read(&sName.at(0), static_cast<int64_t>(sName.size()));
        f1.read(reinterpret_cast<char *>(&sCash), sizeof(sCash));
        f1.read(reinterpret_cast<char *>(&sWins), sizeof(sWins));
        f1.read(reinterpret_cast<char *>(&sLoses), sizeof(sLoses));
        f1.close();
        player.setName(sName);
        player.addCash(sCash - player.getCash());
        while (player.getWins() != sWins) {
            player.incrementWins();
        }
        while (player.getLoses() != sLoses) {
            player.incrementLoses();
        }
    } else {
        beginMenu("File does not exist.");
    }
}

//////////////* Printing Stuff *////

void Game::printStatistics() {
    tui::screen::clear();
    tui::cursor::home();
    std::cout << tui::string(Print::title_blackjack()).yellow() << "\n";
    std::cout << "\n" << tui::string(Print::statistics()).green() << "\n";
    s.print();
    std::cout << "\n\n\t(Press any key to continue)\n";
    read_ch();
}

void Game::printInstructions() {
    tui::screen::clear();
    tui::cursor::home();
    std::cout << tui::string(Print::title_blackjack()).yellow() << "\n";
    std::cout << "\n" << tui::string(Print::instructions()).green() << "\n";
    read_ch();
}

void Game::printTop() {
    tui::screen::clear();
    tui::cursor::home();
    std::cout << tui::string(Print::title_blackjack()).yellow() << "\n";
    std::cout << color::red_fg() << "\t\tCards: " << deck.getSize() << color::green_fg()
              << " \tCash: " << player.getCash() << color::magenta_fg() << " \tBet: " << player.getBet()
              << color::blue_fg() << " \tName: " << player.getName() << style::reset_style() << "\n\n\n";
}

void Game::printBody() {
    printTop();
    std::cout << tui::string(Print::dealer_border()).red();
    dealer.printFirstCard();
    std::cout << tui::string(Print::player_border()).cyan();
    player.printCards();
    std::cout << color::green_fg() << "\nSum: " << color::red_fg() << player.getSum()
              << style::reset_style() << "\n";
}
