#ifndef iMENUACTION_H
#define iMENUACTION_H

/*

Yes, I agree, not the best solution.
But better than it was before.

*/

#include <Misc\NonCopyable.h>
namespace Logic
{
    class StateBuffer;
    class iMenuMachine;
    class Action : public NonCopyable
    {
    public:
        static Action& Get()
        {
            static Action action;
            return action;
        }

        void SetPointer(StateBuffer* stateBuffer) { m_stateBuffer = stateBuffer; }
        void SetPointer(iMenuMachine* menuMachine) { m_menuMachine = menuMachine; }
        
        StateBuffer* m_stateBuffer;
        iMenuMachine* m_menuMachine;
    };

    typedef void(*ButtonFunc)();

    namespace ButtonFunction
    {
        void startGame();
        void startSettings();
        void startMainMenu();
        void showHighscore();
        void quitGame();
        void writing();
        void chooseUpgrade1();
        void chooseUpgrade2();
        void chooseUpgrade3();
        void confirmSkillPicks();
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
    }
}

#endif // !iMENUACTION_H