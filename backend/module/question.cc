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

    if (questionNumber < MAX_ROUND)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 3);

        answer = nouns[dis(gen)];
    }
}

// std::string vec2str(const std::vector<std::string> &vec)
// {
//     std::stringstream ss;
//     ss << "[";
//     for (size_t i = 0; i < vec.size(); ++i)
//     {
//         ss << "\"" << vec[i] << "\"";
//         if (i != vec.size() - 1)
//         {
//             ss << ", ";
//         }
//     }
//     ss << "]";
//     return ss.str();
// }

// std::vector<std::string> str2vec(const std::string &str)
// {
//     std::vector<std::string> result;
//     std::stringstream ss(str);
//     std::string item;
//
//     while (std::getline(ss, item, ','))
//     {
//         item.erase(0, item.find_first_not_of(" \t\r\n\""));
//         item.erase(item.find_last_not_of(" \t\r\n\"") + 1);
//         result.push_back(item);
//     }
//
//     return result;
// }

std::string Question::judge(std::string answer)
{
    LOGGER("Question", "judge start.");

    std::string prompt = "簡単な英語ゲームをしましょう ルールは簡単で私が文章を渡すのでその順序で_に入る単語を1つ回答しなさい また回答は単語のみ出力し改行空白は入れないでください (例: orange) " + answer;

    LOGGER("Question", "SendPrompt: " << prompt);

    std::string res = Gemini::req(prompt);

    LOGGER("Question", "judge finish : Gemini response => " << res);

    return res;
}
