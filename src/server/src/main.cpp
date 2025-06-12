#include <cctype>
#include <iostream>
#include <string>
#include <thread>

#include "crow/app.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "gameManager.hpp"
#include "print.hpp"

GameManager manager;

void gameloop();

int main() {
    crow::SimpleApp app;
    // manager.dealDealer();

    // GET /join?username=...
    CROW_ROUTE(app, "/join").methods("GET"_method)([](const crow::request& req) {
        const char* username = req.url_params.get("username");
        if (!username) {
            return crow::response(400, "Missing username");
        }
        manager.lock();

        auto [p, joined_already] = manager.join_game(username);
        if (joined_already) {
            return crow::response(400, "already joined");
        }
        // manager[p]
        // p->game.dealDealer();
        p->game.deal1_dealer();
        p->game.deal1_player();
        p->game.deal1_dealer();
        p->game.deal1_player();
        // p->game.checkWins();
        // p->addCard(manager.game.deal1());

        crow::json::wvalue res;
        // res["player_id"] = p->getId();
        res["name"] = p->game.player.getName();
        // res["waiting"] = p->getIsWaiting();
        // res["cards"] = p->game.player.dbg_cards();
        return crow::response(res);
    });

    // POST /bet?player_id=...
    CROW_ROUTE(app, "/bet/<string>")
        .methods("POST"_method)([](const crow::request& req, const std::string name) {
            const char* amount_s = req.url_params.get("amount");
            if (!amount_s) {
                return crow::response(400, "missing amount of bet");
            }
            int amount = std::stoi(amount_s);
            auto& player = manager.players[name].game.player;
            player.setBet(amount);

            crow::json::wvalue res;
            // res["ok"] = true;
            res["cash"] = player.getBet();
            return crow::response(res);
        });

    // POST /move?player_id=...
    CROW_ROUTE(app, "/move/<string>")
        .methods("POST"_method)([](const crow::request& req, const std::string& name) {
            const char* action = req.url_params.get("action");
            if (!action) {
                return crow::response(400, "action");
            }
            std::string action_s = std::string(action);
            auto& game = manager.players[name].game;
            if (game.checkWins()) {
                return crow::response(400, "game finished");
            }
            if (action_s == "hit") {
                game.deal1_dealer();
                game.deal1_player();
            } else if (action_s == "stand") {
                std::cout << "standing...";
            } else {
                return crow::response(400, "should 'hit' or 'stand'");
            }
            crow::json::wvalue res;
            if (game.checkWins()) {
                res["winner"] = std::format("{}", game.checkEnd());
            }

            // manager.player_move(pid);

            res["ok"] = true;
            return crow::response(res);
        });

    // GET /game_state
    CROW_ROUTE(app, "/game_state").methods("GET"_method)([](const crow::request&) {
        return crow::response(manager.get_game_state());
    });

    // GET /help
    CROW_ROUTE(app, "/help").methods("GET"_method)([](const crow::request&) {
        return crow::response(Print::instructions());
    });

    auto server = [&app]() { app.port(18080).multithreaded().run(); };
    std::thread server_thread(server);

    gameloop();

    server_thread.join();
}

void gameloop() {
    std::cout << "start game loop\n";
    char ch = 0;
    do {
        // for (const auto &x : manager.get_game_state());
        for (const auto& [id, p] : manager.players) {
            std::cout << "waiting for bet from: " << p.game.player.getName() << "\n";
            if (p.game.player.getBet() != 0) {
                std::cout << "got bet.";
            }
            // manager.game.beginGame();
        }
        std::cout << "cycled, reading:\n";
        ch = read_ch();
    } while (ch != 'q' && ch != 3);

    std::cout << "end game loop\n";
}
