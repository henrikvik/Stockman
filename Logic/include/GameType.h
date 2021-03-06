#ifndef GAME_TYPE_H
#define GAME_TYPE_H

namespace Logic
{
    enum StateType
    {
        Nothing = 0,
        State_Start,
        State_Playing,
        State_InGame_Overlay
    };

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
        {"LOG_GODMODE", "AI_SETMODE 0", "LOG_setGameState 0"},
        {},
        {"AI_SETMODE 2"},
        {"AI_SETMODE 3"},
    };
};

#endif