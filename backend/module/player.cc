#include "player.h"
#include "utils.h"

// TODO errorhandling
std::string Player::create()
{
    std::string player_id = gen_id();

    mtx.lock();

    std::shared_ptr<Player> player(new Player);

    player->id = player_id;
    player->isReady = false;

    players[player_id] = player;

    mtx.unlock();

    return player_id;
}

void Player::rename(std::string player_id, std::string name) {
    try {
        mtx.lock();
        players[player_id]->name = name;
        mtx.unlock();
    } catch {
        mtx.unlock();
        throw "TODO Err";
    }
}
