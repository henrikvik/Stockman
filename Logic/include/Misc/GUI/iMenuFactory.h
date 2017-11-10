#ifndef IMENUFACTORY_H
#define IMENUFACTORY_H

#include <Misc\GUI\iMenu.h>
#include <Misc\NonCopyable.h>

namespace Logic
{
    class iMenuFactory : public NonCopyable
    {
    public:
        iMenuFactory() { }
        ~iMenuFactory() { }

        iMenu* buildMenuStart();
        iMenu* buildMenuSettings();
        iMenu* buildMenuSkill();
        iMenu* buildMenuCard();
        iMenu* buildMenuHighscore();
        iMenu* buildMenuGameover();
    };
}

#endif // !IMENUFACTORY_H
