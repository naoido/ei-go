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

            Json::Value global_dispatch;
            global_dispatch["type"] = "update";
            global_dispatch["playerId"] = s.player_id;

            if (room->state == GameState::Ready)
            {
                room->mtx.lock();
                try
                {
                    std::shared_ptr<Player> player = room->players[s.player_id];

                    if (json_req.isMember("name"))
                    {
                        std::string name = json_req["name"].asCString();

                        player->name = name;
                        global_dispatch["name"] = name;
                    }

                    if (json_req.isMember("isReady"))
                    {
                        bool is_ready = json_req["isReady"].asBool();

                        player->is_ready = is_ready;
                        global_dispatch["isReady"] = is_ready;
                    }

                    json_res["isUpdated"] = true;
                }
                catch (...)
                {
                    ERR_LOGGER("Stream", "Update failed.");
                    json_res["isUpdated"] = false;
                }

                ps_service.publish(s.player_id, json_stringify(json_res));

                std::string str_global_dispatch = json_stringify(global_dispatch);

                for (const auto &player : room->players)
                    if (s.player_id != player.first)
                        ps_service.publish(player.second->id, str_global_dispatch);

                room->mtx.unlock();
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

                        for (int idx = 0; idx < results.size(); idx++)
                        {
                            std::shared_ptr<Player> player = room->players[results[idx].first];
                            std::string ai_guess = results[idx].second;

                            if (ai_guess == room->question->answer)
                                player->point += 10000 * std::exp(-0.1 * idx);

                            json_result["result"]["ranking"][idx]["id"] = player->id;
                            json_result["result"]["ranking"][idx]["name"] = player->name;
                            json_result["result"]["ranking"][idx]["point"] = player->point;
                            json_result["result"]["ranking"][idx]["aiGuess"] = ai_guess;
                            json_result["result"]["ranking"][idx]["isCorrect"] = ai_guess == room->question->answer;
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
    std::string player_id = req->getParameter("playerId");
    std::shared_ptr<Room> room;

    bool is_reconnection = false;

    try
    {
        room = Room::get(room_id);

        /* #-#-#-#-#-# Reconnection process #-#-#-#-#-# */
        if (player_id != "")
        {
            if (!room->players.count(player_id))
            {
                ERR_LOGGER("Stream", "non-existent player_id -> " << player_id);
                throw "";
            }

            is_reconnection = true;
        }
        else
            player_id = room->join();

        Json::Value json_res;
        json_res["type"] = "gameJoined";
        json_res["playerId"] = player_id;

        conn->send(json_stringify(json_res));

        int idx = 0;
        for (const auto player : room->players)
        {
            json_res["users"][idx]["id"] = player.second->id;
            json_res["users"][idx]["name"] = player.second->name;
            json_res["users"][idx]["is_ready"] = player.second->is_ready;

            idx++;
        }

        conn->send(json_stringify(json_res));

        /* #-#-#-#-#-#  Notify other players  #-#-#-#-#-# */
        if (!is_reconnection)
        {
            Json::Value global_dispatch;
            global_dispatch["type"] = "new_player";
            global_dispatch["playerId"] = player_id;
            global_dispatch["name"] = room->players[player_id]->name;
            global_dispatch["is_ready"] = room->players[player_id]->is_ready;

            std::string str_global_dispatch = json_stringify(global_dispatch);

            for (const auto &player : room->players)
                if (player.first != player_id)
                    ps_service.publish(player.first, str_global_dispatch);
        }
    }
    catch (...)
    {
        Json::Value json_res;
        json_res["type"] = "joinFailed";
        json_res["err"] = "TODO Err";

        conn->send(json_stringify(json_res));
        conn->shutdown();
    }

    Subscriber s;
    s.room_id = room_id;
    s.player_id = player_id;
    s.sub_id = ps_service.subscribe(s.player_id == "" ? Utils::gen_id() : s.player_id, [conn](
                                                                                           const std::string &topic,
                                                                                           const std::string &message)
                                    { conn->send(message); });

    LOGGER("Stream", "Subscribe sub_id: " << s.sub_id);

    conn->setContext(std::make_shared<Subscriber>(std::move(s)));
}

void WebSocketChat::handleConnectionClosed(const WebSocketConnectionPtr &conn)
{
    try
    {
        auto &s = conn->getContextRef<Subscriber>();

        if (s.player_id != "")
        {
            ps_service.unsubscribe(s.player_id, s.sub_id);
            WARN_LOGGER("Stream", "Disconnected. \n  >> room_id: " << s.room_id << "\n  >> player_id: " << s.player_id);

            if (s.room_id != "")
            {
                std::shared_ptr<Room> room = Room::get(s.room_id);

                if (room->players.count(s.player_id))
                {
                    room->players[s.player_id]->is_ready = false;

                    Json::Value global_dispatch;
                    global_dispatch["type"] = "update";
                    global_dispatch["playerId"] = s.player_id;
                    global_dispatch["is_ready"] = false;

                    std::string str_global_dispatch = json_stringify(global_dispatch);

                    for (const auto &player : room->players)
                        if (player.first != s.player_id)
                            ps_service.publish(player.first, str_global_dispatch);
                }
            }
        }
    }
    catch (...)
    {
        ERR_LOGGER("Stream", "Some error occurred when disconnecting");
    }
}
