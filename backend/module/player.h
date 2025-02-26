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
    bool is_ready;


private:
    static std::map<std::string, std::shared_ptr<Player>> player;
    static std::mutex mtx;
}
