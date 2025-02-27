#include "gemini.h"

const std::string API_KEY = std::getenv("GEMINI_API_KEY");
const std::string GEMINI_API_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + API_KEY;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
}

std::string Gemini::req(std::string prompt)
{
    CURL *curl;
    CURLcode res;
    std::string response_data;

    LOGGER("Gemini", "Gemini request.");

    curl = curl_easy_init();
    if (curl)
    {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        Json::Value request_body;
        request_body["contents"][0]["parts"][0]["text"] = prompt; //

        std::string post_data = json_stringify(request_body);

        curl_easy_setopt(curl, CURLOPT_URL, GEMINI_API_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        Json::Value response;
        json_parse(response, response_data);

        std::string result = json_stringify(response["candidates"][0]["content"]["parts"][0]["text"]);

        {
            std::string target = "\\n";
            size_t pos = result.find(target);

            while (pos != std::string::npos)
            {
                result.erase(pos, target.length());
                pos = result.find(target);
            }
        }

        {
            std::string target = "\n";
            size_t pos = result.find(target);

            while (pos != std::string::npos)
            {
                result.erase(pos, target.length());
                pos = result.find(target);
            }
        }

        {
            std::string target = "\"";
            size_t pos = result.find(target);

            while (pos != std::string::npos)
            {
                result.erase(pos, target.length());
                pos = result.find(target);
            }
        }

        std::transform(result.begin(), result.end(), result.begin(), ::tolower);

        return result;
    }
}
