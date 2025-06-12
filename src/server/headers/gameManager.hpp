#include <mutex>
#include <random>
#include <string>
#include <unordered_map>

#include "crow/json.h"
#include "game.hpp"
#include "player.hpp"

// === Basic Models ===
class ServerPlayer final : public Player {
   public:
    [[nodiscard]] bool getMoveMade() const { return this->move_made; };
    [[nodiscard]] bool getIsWaiting() const { return this->is_waiting; };
    [[nodiscard]] std::string getId() const { return this->id; };

    void setMoveMade(const bool& move_made) { this->move_made = move_made; };
    void setIsWaiting(const bool& is_waiting) { this->is_waiting = is_waiting; };
    void setId(std::string id) { this->id = std::move(id); };

   private:
    std::string id;
    bool move_made = false;
    bool is_waiting = false;
};

// === Global Game Manager ===
class GameManager {
   private:
    std::mutex mtx;

   public:
    // Game game;
    std::unordered_map<std::string, ServerPlayer> players;
    enum Status : uint8_t { WAITING, IN_PROGRESS } status = WAITING;

    void lock() {
        std::lock_guard lock(this->mtx);
    }

    static std::string generate_id() {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_int_distribution<int> dist(0, 15);
        std::string id;
        for (int i = 0; i < 8; ++i) {
            id += std::string("0123456789abcdef").at(dist(rng));
        }
        return id;
    }

    ServerPlayer* join_game(const std::string& name) {
        this->lock();

        // Check if player already exists
        // for (auto& [id, player] : game.players) {
        //     if (player.getName() == name) {
        //         return &player;
        //     }
        // }

        // Add new player
        std::string new_id = generate_id();
        ServerPlayer p;
        p.setName(name);
        p.setId(new_id);
        p.setIsWaiting(status == IN_PROGRESS);
        p.setMoveMade(false);

        players.insert({new_id, p});
        return &players[new_id];
    }

    crow::json::wvalue get_game_state() {
        this->lock();
        crow::json::wvalue res;

        res["status"] = (status == WAITING) ? "waiting" : "in_progress";
        int idx = 0;
        for (const auto& [id, p] : players) {
            res["players"][idx]["id"] = id;
            res["players"][idx]["name"] = p.getName();
            res["players"][idx]["move_made"] = p.getMoveMade();
            res["players"][idx]["waiting"] = p.getIsWaiting();
            idx++;
        }
        return res;
    }

    void reset_round_if_all_moved() {
        this->lock();
        if (players.empty()) {
            return;
        }

        bool all_moved = true;
        for (const auto& [_, p] : players) {
            if (!p.getMoveMade() && !p.getIsWaiting()) {
                all_moved = false;
            }
        }

        if (all_moved) {
            for (auto& [_, p] : players) {
                p.setMoveMade(false);
                if (p.getIsWaiting()) {
                    p.setIsWaiting(false);
                }
            }
        }
    }

    void player_move(const std::string& pid) {
        this->lock();
        auto it = players.find(pid);
        if (it != players.end()) {
            it->second.setMoveMade(true);
        }
        reset_round_if_all_moved();
    }
};
