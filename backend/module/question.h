#pragma once

#include <drogon/drogon.h>
#include <random>

class Question
{
public:
    std::string answer;
    std::map<std::string, std::string> players_answer;

    Question();

    bool isAllAnswered();
    std::map<std::string, std::string> judge();

private:
    static const std::array<std::string, 300> nouns;
};
