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

bool Question::isAllAnswered()
{
    for (const auto &answer : players_answer)
        if (answer.second == "")
            return false;

    return true;
}

std::map<std::string, std::string> judge() {
    
}
