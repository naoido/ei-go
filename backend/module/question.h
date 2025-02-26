#pragma once

#include <drogon/drogon.h>
#include <random>

#include "gemini.h"


class Question
{
public:
    std::string answer;
    std::map<std::string, std::string> players_answer;
    int questionNumber = 0;

    Question();

    void next();
    std::vector<std::pair<std::string, std::string>> judge();

private:
    static const std::array<std::string, 300> nouns;
};
