#include "question.h"

const std::array<std::string, 4> Question::nouns = {"apple", "banana", "car", "dog"};

Question::Question()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nouns.size() - 1);

    answer = nouns[dis(gen)];
}

void Question::next()
{
    questionNumber++;

    if (questionNumber < 5)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 3);

        answer = nouns[dis(gen)];
    }
}

std::string vec2str(const std::vector<std::string> &vec)
{
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        ss << "\"" << vec[i] << "\"";
        if (i != vec.size() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

std::vector<std::string> str2vec(const std::string &str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ','))
    {
        item.erase(0, item.find_first_not_of(" \t\r\n\""));
        item.erase(item.find_last_not_of(" \t\r\n\"") + 1);
        result.push_back(item);
    }

    return result;
}

std::vector<std::pair<std::string, std::string>> Question::judge()
{
    LOGGER("Question", "judge start.");

    std::vector<std::string> player_ids;
    std::vector<std::string> answers;

    for (const auto &pair : players_answer)
    {
        player_ids.push_back(pair.first);
        answers.push_back(pair.second);
    }

    std::string prompt = "簡単な英語ゲームをしましょう ルールは簡単で私がString[]型の文章を渡すのでその順序で_に入る単語1つをカンマ区切りにして回答しなさい また回答のみを出力し改行空白は入れないでください (回答例: \"apple\", \"orange\"...) " + vec2str(answers);

    LOGGER("Question", "SendPrompt: " << prompt);

    std::string res = Gemini::req(prompt);

    std::vector<std::string> ai_answers = str2vec(res);
    std::vector<std::pair<std::string, std::string>> result;

    LOGGER("Question", "Gemini response: " << res);

    for (size_t i = 0; i < player_ids.size() && i < ai_answers.size(); ++i)
        result.push_back({player_ids[i], ai_answers[i]});

    LOGGER("Question", "judge finish.");

    return result;
}
