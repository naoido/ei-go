#include "room.h"
#include "question.h"


std::shared_ptr<Room> Room::create()
{
    std::string room_id = Utils::gen_id();
    std::string admin_token = Utils::gen_id();
    std::map<std::string, std::shared_ptr<Player>> players;
    GameState game_state = GameState::Ready;
    std::shared_ptr<Question> question(new Question);

    std::shared_ptr<Room> room(new Room);

    room->id = room_id;
    room->state = game_state;
    room->players = players;
    room->admin_token = admin_token;
    room->question = question;

    rooms[room_id] = room;

    LOGGER("Room", "created room: " << room->id);

    return room;
}

std::string Room::join()
{
    std::shared_ptr<Player> player(new Player);
    players[player->id] = player;

    return player->id;
}

std::shared_ptr<Room> Room::get(const std::string &room_id)
{
    if (!(rooms.count(room_id))) {
        ERR_LOGGER("Room", "invalid id");
        throw "TODO Err";
    }

    return rooms[room_id];
}

void Room::dispose(const std::string &room_id)
{
    auto it = rooms.find(room_id);
    if (it != rooms.end())
        rooms.erase(it);

    return;
}

bool Room::verify_token(const std::string &token)
{
    return admin_token == token;
}

bool Room::isAllPlayersReady()
{
    mtx.lock();
    try
    {
        for (const auto &player : players)
            if (!player.second->is_ready)
                return false;

        mtx.unlock();
        return true;
    }
    catch (...)
    {
        mtx.unlock();
        return false;
    }
}

std::map<std::string, std::shared_ptr<Room>> Room::rooms;