#pragma once

#include <drogon/drogon.h>
#include <mutex>
#include <map>

/* ----- use modules include -----*/
#include "utils.h"

typedef std::string RoomId;

enum GameState
{
    Ready,
    AcceptingAnswers,
    Judging,
    RevealingResults,
    End
};

class Room
{
public:
    RoomId id;
    GameState state;
    std::vector<std::shared_ptr<Player>> players;
    std::string admin_token;
    std::mutex mtx_;

    /**
     * @return (room_id, admin_token)を返します
     */
    static std::pair<std::string, std::string> create();
    static std::string join(const std::string room_id);
    static std::shared_ptr<Room> get(const std::string room_id);
    static void change_state(const std::string room_id, GameState state);
    static void dispose(const std::string room_id);

    bool verify_token(std::string token);

private:
    static std::map<std::string, std::shared_ptr<Room>> rooms;
    static std::mutex mtx;
}
