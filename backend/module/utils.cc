const long int getTimeStamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

const std::string gen_id()
{
    std::string i;
    int x;

    for (long int t = getTimeStamp(); t; t /= 36)
    {
        x = t % 36;
        i = (char)(x < 10 ? '0' + x : 'A' + x - 10) + i;
    }

    return i += (char)('A' + rand() % 26), i += (char)('A' + rand() % 26);
}