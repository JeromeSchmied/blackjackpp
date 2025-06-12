#include <cctype>
#include <iostream>
#include <string>
#include <thread>

#include "crow/app.h"
#include "crow/json.h"
#include "gameManager.hpp"
#include "print.hpp"

GameManager manager;

void gameloop();

int main() {
    crow::SimpleApp app;

    // GET /join?username=...
    CROW_ROUTE(app, "/join").methods("GET"_method)([](const crow::request& req) {
        const char* username = req.url_params.get("username");
        if (!username) {
            return crow::response(400, "Missing username");
        }

        ServerPlayer* p = manager.join_game(username);

        crow::json::wvalue res;
        res["player_id"] = p->getId();
        res["name"] = p->getName();
        res["waiting"] = p->getIsWaiting();
        return crow::response(res);
    });

    // POST /move?player_id=...
    CROW_ROUTE(app, "/move").methods("POST"_method)([](const crow::request& req) {
        const char* pid = req.url_params.get("player_id");
        if (!pid) {
            return crow::response(400, "Missing player_id");
        }

        manager.player_move(pid);

        crow::json::wvalue res;
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
    do {
        // manager.game.beginGame();
        for (const auto& p : manager.players) {
            std::cout << p.second.getName() << "\n";
        }
        std::cout << "cycled, reading:\n";
    } while (toupper(read_ch()) != 'Q');

    std::cout << "end game loop\n";
}
