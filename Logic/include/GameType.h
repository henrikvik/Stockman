#ifndef GAME_TYPE_H
#define GAME_TYPE_H

namespace Logic
{
    static const int GAME_TYPES = 4, MAX_COMMANDS = 10;
    enum GameType
    {
        TESTING_MODE,
        NORMAL_MODE,
        HARDCORE_MODE,
        DARK_SOULS,
    };
    static const std::wstring GameTypeStr[] = 
    {
        L"TESTING_MODE",
        L"NORMAL_MODE",
        L"HARDCORE_MODE",
        L"DARK_SOULS"
    };
    static const std::string GameCommands[GAME_TYPES][MAX_COMMANDS] =
    {
        {"GODMODE", "setAI 0", "setGameState 0"},
        {},
        {"setAI 2"},
        {"setAI 3"},
    };
};

#endif