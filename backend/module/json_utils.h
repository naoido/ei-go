#pragma once

#include <drogon/drogon.h>

#include "logger.h"

/* ----- prototype declaration ----- */
Json::Value* json_parse(Json::Value &root, const std::string &message);
std::string json_stringify(Json::Value &root);