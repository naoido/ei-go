#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "json_utils.h"

#include <drogon/drogon.h>
#include <curl/curl.h>

namespace Gemini {
    std::string req(std::string prompt);
}
