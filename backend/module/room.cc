#include "room.h"
#include "utils.h"

std::shared_ptr<Room> Room::create()
{
    std::string room_id = Utils::gen_id();
    std::string admin_token = Utils::gen_id();
    std::vector<std::string> players_id;
    GameState game_state = GameState::Ready;

    std::shared_ptr<Room> room(new Room);

    room->id = room_id;
    room->state = game_state;
    room->players_id = players_id;
    room->admin_token = admin_token;

    rooms[room_id] = room;

    return room;
}

std::string Room::join()
{
    std::shared_ptr<Player> player(new Player);
    players.push_back(player);

    return player->id;
}

std::shared_ptr<Room> Room::get(const std::string room_id)
{
    if (!(rooms.count(room_id)))
        throw "TODO Err";

    return ROOMS[room_id];
}

void Room::dispose(const std::string room_id)
{
    mtx.lock();

    auto it = map.find(room_id);
    if (it != map.end())
    {
        map.erase(it);
    }

    mtx.unlock();

    return;
}

bool Room::verify_token(std::string token) {
    return admin_token == token;
}
