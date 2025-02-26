#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include "json_utils.h"
#include "logger.h"
#include "room.h"
#include "player.h"
#include "question.h"

using namespace drogon;

struct Subscriber
{
    drogon::SubscriberID sub_id;
    std::string player_id;
    std::string room_id;
};

class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
public:
    virtual void handleNewMessage(
        const WebSocketConnectionPtr &,
        std::string &&,
        const WebSocketMessageType &) override;

    virtual void handleConnectionClosed(
        const WebSocketConnectionPtr &) override;

    virtual void handleNewConnection(
        const HttpRequestPtr &,
        const WebSocketConnectionPtr &) override;

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/api/stream", Get);
    WS_PATH_LIST_END

private:
    PubSubService<std::string> ps_service;
};
