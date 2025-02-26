#include "api.h"

void api::room_create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value json_res;

    LOGGER("API", "POST");

    try
    {
        std::shared_ptr<Room> room = Room::create();

        json_res["roomId"] = room->id;
        json_res["admin_token"] = room->admin_token;

        auto res = HttpResponse::newHttpJsonResponse(json_res);
        res->setStatusCode(k200OK);
        callback(res);
    }
    catch (...)
    {
        Json::Value json_res;
        json_res["err"] = "InternalServerError";

        auto res = HttpResponse::newHttpJsonResponse(json_res);
        res->setStatusCode(k500InternalServerError);
        callback(res);
    }

};