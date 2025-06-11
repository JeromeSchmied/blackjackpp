#include "headers/statistics.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

#include "tui.hpp"

namespace color = tui::text::color;
namespace style = tui::text::style;

//////////////* Default Constructor *////

PlayerSet::PlayerSet() : name("N/A"), cash(1000), wins(0), loses(0) {}

//////////////* Getter Functions *////

// Returns name of Player
std::string PlayerSet::getName() { return this->name; }

// Returns cash of Player
int PlayerSet::getCash() const { return this->cash; }

// Returns wins of Player
int PlayerSet::getWins() const { return this->wins; }

// Returns loses of Player
int PlayerSet::getLoses() const { return this->loses; }

//////////////* Setter Function *////

void PlayerSet::setValues(std::string nm, int c, int w, int l) {
    this->name = std::move(nm);
    this->cash = c;
    this->wins = w;
    this->loses = l;
}

//////////////////////////////////////////////////////////////////

//////////////* Default Constructor *////

Statistics::Statistics() {
    std::fstream temp;
    temp.open("data/statistics.bin", std::ios::in | std::ios::binary);
    if (temp.fail()) {
        saveStats();
    } else {
        temp.close();
        loadStats();
    }
}

//////////////* Checks for High Score & Saves *////

bool Statistics::check(Player pl) {
    bool rewrite = false;
    if (pl.getCash() > this->p.at(0).getCash()) {
        this->p.at(0).setValues(pl.getName(), pl.getCash(), pl.getWins(), pl.getLoses());
        rewrite = true;
    }
    if (pl.getWins() > this->p.at(1).getWins()) {
        this->p.at(1).setValues(pl.getName(), pl.getCash(), pl.getWins(), pl.getLoses());
        rewrite = true;
    }
    if (pl.getLoses() > this->p.at(2).getLoses()) {
        this->p.at(2).setValues(pl.getName(), pl.getCash(), pl.getWins(), pl.getLoses());
        rewrite = true;
    }
    if (rewrite) {
        saveStats();
    }
    return rewrite;
}

//////////////* Printing *////

void Statistics::print() {
    int maxlength = static_cast<int>(
        std::max({this->p.at(0).getName().length(), this->p.at(0).getName().length(), this->p.at(0).getName().length()}));
    for (int i = 0; i < 3; i++) {
        switch (i) {
            case 0:
                std::cout << "MAX CASH  ||||||||| ";
                break;
            case 1:
                std::cout << "MAX WINS  ||||||||| ";
                break;
            case 2:
                std::cout << "MAX LOSES ||||||||| ";
                break;
            default:
                break;
        }
        std::cout << std::setw(maxlength + 1) << this->p.at(i).getName() << "\t | \t" << color::green_fg()
                  << "Cash: " << std::setw(7) << this->p.at(i).getCash() << "\t | \t" << color::yellow_fg()
                  << "Wins: " << std::setw(5) << this->p.at(i).getWins() << "\t | \t" << color::red_fg()
                  << "Loses: " << std::setw(5) << this->p.at(i).getLoses() << style::reset_style() << "\n";
    }
}

//////////////* File Handling *////

void Statistics::saveStats() {
    std::fstream f1;
    f1.open("data/statistics.bin", std::ios::out | std::ios::binary);
    for (int i = 0; i < 3; i++) {
        std::string sName = this->p.at(i).getName();
        int nameSize = static_cast<int>(sName.size());
        int sCash = this->p.at(i).getCash();
        int sWins = this->p.at(i).getWins();
        int sLoses = this->p.at(i).getLoses();
        f1.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        f1.write(sName.c_str(), static_cast<int64_t>(sName.size()));
        f1.write(reinterpret_cast<char*>(&sCash), sizeof(sCash));
        f1.write(reinterpret_cast<char*>(&sWins), sizeof(sWins));
        f1.write(reinterpret_cast<char*>(&sLoses), sizeof(sLoses));
    }
    f1.close();
}

void Statistics::loadStats() {
    std::fstream f1;
    f1.open("data/statistics.bin", std::ios::in | std::ios::binary);
    for (int i = 0; i < 3; i++) {
        std::string sName;
        int nameSize = 0;
        int sCash = 0;
        int sWins = 0;
        int sLoses = 0;
        f1.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        sName.resize(nameSize);
        f1.read(&sName.at(0), static_cast<int64_t>(sName.size()));
        f1.read(reinterpret_cast<char*>(&sCash), sizeof(sCash));
        f1.read(reinterpret_cast<char*>(&sWins), sizeof(sWins));
        f1.read(reinterpret_cast<char*>(&sLoses), sizeof(sLoses));
        this->p.at(i).setValues(sName, sCash, sWins, sLoses);
    }
    f1.close();
}
