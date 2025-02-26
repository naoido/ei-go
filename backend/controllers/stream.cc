#include "stream.h"

void WebSocketChat::handleNewMessage(
    const WebSocketConnectionPtr &wsConnPtr,
    std::string &&message,
    const WebSocketMessageType &type)
{
    auto &s = wsConnPtr->getContextRef<Subscriber>();

    if (
        type == WebSocketMessageType::Binary ||
        type == WebSocketMessageType::Close ||
        type == WebSocketMessageType::Ping ||
        type == WebSocketMessageType::Pong ||
        type == WebSocketMessageType::Unknown)
    {
        WARN_LOGGER("Stream", "Messages not processed");
        return;
    }
    else
        LOGGER("Stream", "New Message: \n\t" << message);

    try
    {
        Json::Value json_req;
        json_parse(json_req, message);

        std::string event_type = json_req["type"].asCString();
        std::shared_ptr<Room> room = Room::get(s.room_id);

        if (event_type == "update")
        {
            Json::Value json_res;
            json_res["type"] = "acceptedUpdate";

            if (room->state == GameState::Ready)
            {
                room->mtx.lock();
                try
                {
                    std::shared_ptr<Player> player = room->players[s.player_id];
                    player->name = json_req["name"].asCString();

                    json_res["isUpdated"] = true;
                }
                catch (...)
                {
                    json_res["isUpdated"] = false;
                }

                json_res["type"] = "update";
                json_res["playerId"] = s.player_id;
                json_res["name"] = room->players[s.player_id]->name;

                std::string str_res = json_stringify(json_res);

                for (const auto &player : room->players)
                    ps_service.publish(player.second->id, str_res);

                room->mtx.unlock();
                ps_service.publish(s.player_id, json_stringify(json_res));
            }
        }
        else if (event_type == "ready")
        {
            Json::Value json_res;
            json_res["type"] = "acceptedRaady";

            if (room->state == GameState::Ready)
            {
                room->mtx.lock();
                try
                {
                    std::shared_ptr<Player> player = room->players[s.player_id];
                    player->is_ready = true;
                }
                catch (...)
                {
                }

                json_res["type"] = "update";
                json_res["playerId"] = s.player_id;
                json_res["is_ready"] = room->players[s.player_id]->is_ready;

                std::string str_res = json_stringify(json_res);

                for (const auto &player : room->players)
                    ps_service.publish(player.second->id, str_res);

                room->mtx.unlock();
                ps_service.publish(s.player_id, json_stringify(json_res));
            }
        }
        else if (event_type == "gameStart")
        {
            Json::Value json_res;
            json_res["type"] = "question";

            std::string admin_token = json_req["admin_token"].asCString();

            if (room->state == GameState::Ready && room->isAllPlayersReady() && admin_token == room->admin_token)
            {
                room->mtx.lock();
                room->state = GameState::AcceptingAnswers;

                json_res["question"] = room->question->answer;
                json_res["questionNumber"] = room->question->questionNumber;
                std::string str_res = json_stringify(json_res);

                for (const auto &player : room->players)
                    ps_service.publish(player.second->id, str_res);

                room->mtx.unlock();
            }
        }
        else if (event_type == "answer")
        {
            if (room->state == GameState::AcceptingAnswers)
            {
                Json::Value json_res;
                json_res["type"] = "acceptedAnswer";

                room->mtx.lock();
                try
                {
                    room->question->players_answer[s.player_id] = json_req["answer"].asCString();
                    json_res["isAccepted"] = true;

                    ps_service.publish(s.player_id, json_stringify(json_res));

                    if (room->question->players_answer.size() == room->players.size())
                    {
                        room->state = GameState::Judging;

                        json_res["type"] = "state";
                        json_res["state"] = "judging";

                        std::string str_res = json_stringify(json_res);

                        for (const auto &player : room->players)
                            ps_service.publish(player.second->id, str_res);

                        std::vector<std::pair<std::string, std::string>> results = room->question->judge();

                        Json::Value json_result;
                        json_result["type"] = "result";

                        LOGGER("Stream", "Dev." << results.size());

                        for (int idx = 0; idx < results.size(); idx++)
                        {
                            json_result["result"]["ranking"][idx]["id"] = results[idx].first;
                            json_result["result"]["ranking"][idx]["name"] = room->players[results[idx].first]->name;
                            json_result["result"]["ranking"][idx]["aiGuess"] = results[idx].second;
                            json_result["result"]["ranking"][idx]["isCorrect"] = results[idx].second == room->question->answer;
                        }

                        for (const auto result : results)
                        {
                            json_result["result"]["isCorrect"] = result.second == room->question->answer;
                            json_result["result"]["aiGuess"] = result.second;

                            ps_service.publish(result.first, json_stringify(json_result));
                        }

                        if (room->question->questionNumber < 5)
                        {
                            room->question->next();
                            room->state = GameState::Ready;
                        }
                        else
                        {
                            Json::Value json_res;
                            json_res["type"] = "finish";

                            std::string admin_token = json_req["admin_token"].asCString();

                            if (room->state == GameState::Ready && admin_token == room->admin_token)
                            {
                                std::string str_res = json_stringify(json_res);

                                for (const auto &player : room->players)
                                    ps_service.publish(player.second->id, str_res);

                                Room::dispose(room->id);
                            }
                        }
                    }
                }
                catch (...)
                {
                    ERR_LOGGER("Stream", "judge err");
                }
                room->mtx.unlock();
            }
        }
        else if (event_type == "finish")
        {
            Json::Value json_res;
            json_res["type"] = "finish";

            std::string admin_token = json_req["admin_token"].asCString();

            if (room->state == GameState::Ready && admin_token == room->admin_token)
            {
                std::string str_res = json_stringify(json_res);

                for (const auto &player : room->players)
                    ps_service.publish(player.second->id, str_res);

                Room::dispose(room->id);
            }
        }
        else
        {
            WARN_LOGGER("Stream", "invalid event name => " << event_type);
        }
    }
    catch (...)
    {
    }
}

void WebSocketChat::handleNewConnection(
    const HttpRequestPtr &req,
    const WebSocketConnectionPtr &conn)
{
    std::string room_id = req->getParameter("roomId");
    std::shared_ptr<Room> room = Room::get(room_id);
    std::string player_id = room->join();

    try
    {
        Json::Value res;
        res["type"] = "gameJoined";
        res["playerId"] = player_id;

        int idx = 0;
        for (const auto player & : room->players)
        {
            json_res["users"][idx]["id"] = player.second->id;
            json_res["users"][idx]["id"] = player.second->name;
            json_res["users"][idx]["id"] = player.second->is_ready;

            idx++;
        }

        conn->send(json_stringify(res));
    }
    catch (...)
    {
        Json::Value res;
        res["type"] = "joinFailed";
        res["err"] = "TODO Err";

        conn->send(json_stringify(res));
        conn->shutdown();
    }

    Subscriber s;
    s.room_id = room_id;
    s.player_id = player_id;
    s.sub_id = ps_service.subscribe(s.player_id, [conn](
                                                     const std::string &topic,
                                                     const std::string &message)
                                    { conn->send(message); });

    LOGGER("Stream", "sub ok. sub_id: " << s.sub_id);

    conn->setContext(std::make_shared<Subscriber>(std::move(s)));

    Json::Value json_res;
    json_res["type"] = "new_player";
    json_res["playerId"] = s.player_id;
    json_res["name"] = room->players[s.player_id]->name;
    json_res["is_ready"] = room->players[s.player_id]->is_ready;

    std::string str_res = json_stringify(json_res);

    for (const auto &player : room->players)
        if (player.first != s.player_id)
            ps_service.publish(player.first, str_res);
}

void WebSocketChat::handleConnectionClosed(const WebSocketConnectionPtr &conn)
{
    auto &s = conn->getContextRef<Subscriber>();
    ps_service.unsubscribe(s.player_id, s.sub_id);
    WARN_LOGGER("Stream", "Disconnected. \n  >> room_id: " << s.room_id << "\n  >> player_id: " << s.player_id);
}
