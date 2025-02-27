#pragma once

#include <drogon/drogon.h>
#include <random>

#include "gemini.h"

#define MAX_ROUND 3

struct PlayerAnswer {
    std::string answer;
    std::string ai_guess;
    bool is_correct;
};

class Question
{
public:
    std::string answer;
    std::map<std::string, PlayerAnswer> players_answer;
    int questionNumber = 0;

    Question();

    void next();
    std::string judge(std::string answer);

private:
    static const std::array<std::string, 4> nouns;
};
