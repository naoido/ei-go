#include <drogon/drogon.h>
#include "logger.h"

using namespace drogon;

int main()
{
    // Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5173);
    // Load config file
    drogon::app().loadConfigFile("../config.json");
    drogon::app().registerPostHandlingAdvice(
        [](const HttpRequestPtr &req, const HttpResponsePtr &res)
        {
            res->addHeader("Access-Control-Allow-Origin", "*");
        });
    LOGGER("Main", "Listen 0.0.0.0:5173");
    drogon::app().run();

    return 0;
}
