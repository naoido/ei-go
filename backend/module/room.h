#pragma once

#include <drogon/drogon.h>
#include <iostream>
#include <mutex>
#include <map>

/* ----- use modules include -----*/
#include "utils.h"
#include "player.h"
#include "question.h"

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
    std::shared_ptr<Question> question;
    std::map<std::string, std::shared_ptr<Player>> players;
    std::string admin_token;
    std::mutex mtx;


    static std::shared_ptr<Room> create();    
    static std::shared_ptr<Room> get(const std::string& room_id);
    static void dispose(const std::string& room_id);

    std::string join();

    bool verify_token(const std::string& token);
    bool isAllPlayersReady();

private:
    static std::map<std::string, std::shared_ptr<Room>> rooms;
};
