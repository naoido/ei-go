#pragma once

#include <drogon/drogon.h>
#include <mutex>
#include <map>

/* ----- use modules include -----*/
#include "utils.h"


class Player
{
public:
    std::string id;
    std::string name;
    bool isReady;

    std::string create();
    std::string rename(std::string player_id, std::string name);

private:
    static std::map<std::string, std::shared_ptr<Player>> player;
    static std::mutex mtx;
}
