#ifndef iMENUACTION_H
#define iMENUACTION_H

#include <Misc\NonCopyable.h>
namespace Logic
{
    typedef void(*ButtonFunc)();

    class iMenuAction : public NonCopyable
    {
    public:
        void startGame();
        void startSettings();
        void startMainMenu();
        void quitGame();
        void writing();
        void chooseUpgrade1();
        void chooseUpgrade2();
        void chooseUpgrade3();
        void chooseSkill1();
        void chooseSkill2();
        void chooseSkill3();
        void plusSense();
        void minusSense();
        void plusMaster();
        void minusMaster();
        void plusSFX();
        void minusSFX();
        void muteUnmute();
        void plusFOV();
        void minusFOV();
        void windowed();
        void showHighscore();
    };
}

#endif // !iMENUACTION_H