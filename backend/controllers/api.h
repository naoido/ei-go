#pragma once

#include <drogon/HttpController.h>
#include <iostream>
#include <compare>

#include "json_utils.h"
#include "room.h"
#include "logger.h"

using namespace drogon;

class api : public drogon::HttpController<api>
{
  public:
    void room_create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr &)> &&callback);

    METHOD_LIST_BEGIN
    METHOD_ADD(api::room_create, "/room/create", Post, Options);
    METHOD_LIST_END
};