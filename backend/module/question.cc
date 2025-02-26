#include "question.h"

const std::array<std::string, 300> Question::nouns = {
    "apple", "banana", "car", "dog", "elephant", "fish", "guitar", "house", "ice", "jacket",
    "kangaroo", "lamp", "mountain", "notebook", "orange", "pencil", "quilt", "rose", "sun", "tree",
    "umbrella", "violet", "window", "xylophone", "yarn", "zebra", "airport", "bottle", "chair", "desk",
    "egg", "fan", "game", "hat", "insect", "jungle", "key", "leaf", "moon", "net",
    "ocean", "piano", "question", "rabbit", "star", "tiger", "unicorn", "volcano", "whale", "x-ray",
    "yellow", "zoo", "acorn", "bridge", "cloud", "door", "elephant", "forest", "garden", "house",
    "island", "jungle", "kitchen", "lake", "mountain", "nest", "ocean", "pillow", "queen", "river",
    "street", "train", "umbrella", "village", "waterfall", "xenon", "yard", "zodiac", "airplane",
    "beach", "candle", "drum", "eggplant", "fishbowl", "goat", "hammock", "ink", "jacket", "keyboard",
    "lamp", "moonlight", "noodles", "octopus", "peach", "quail", "rain", "sand", "table", "underwear",
    "vampire", "whistle", "xylophone", "yo-yo", "zipper", "atlas", "bar", "cow", "dolphin", "ear",
    "fox", "grape", "hat", "indigo", "jam", "kettle", "lemon", "mug", "nest", "owl", "parrot", "queen",
    "rose", "shoe", "tree", "ugly", "vine", "witch", "xenon", "yellow", "zephyr", "apple", "book", "cup",
    "dog", "elephant", "fan", "grape", "house", "ice", "jewel", "kangaroo", "leaf", "moose", "note",
    "orange", "peach", "quilt", "rocket", "sun", "tiger", "umbrella", "vulture", "window", "xylophone",
    "yellow", "zebra", "account", "bag", "cell", "day", "ear", "fist", "ghost", "hat", "igloo", "joker",
    "knee", "lamp", "moon", "night", "open", "pen", "quick", "rest", "study", "tent", "understand", "vowel",
    "wish", "xerox", "yellowstone", "zebra", "axe", "bell", "coat", "dust", "ear", "flute", "glove", "halo",
    "ice", "jump", "king", "loop", "mock", "net", "oak", "pack", "queen", "rat", "sock", "tennis", "useless",
    "vulture", "watch", "ox", "zebra", "bucket", "bracelet", "clamp", "dove", "echo", "flute", "guitar", "hearth",
    "ion", "joke", "kick", "land", "mouse", "nut", "opponent", "pocket", "question", "rose", "score", "track",
    "user", "vase", "wheel", "ox", "xerox", "yarn", "zone"};

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
        std::uniform_int_distribution<> dis(0, nouns.size() - 1);

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
