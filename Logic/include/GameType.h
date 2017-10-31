#ifndef GAME_TYPE_H
#define GAME_TYPE_H

namespace Logic
{
    enum GameType
    {
        TESTING_MODE,
        NORMAL_MODE,
        HARDCORE_MODE,
        DARK_SOULS,
    };
    static const std::wstring GameTypeStr[] = {
        L"TESTING_MODE",
        L"NORMAL_MODE",
        L"HARDCORE_MODE",
        L"DARK_SOULS"
    };
};

#endif