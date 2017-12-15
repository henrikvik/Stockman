#ifndef iMENUCONTROLS_H
#define iMENUCONTROLS_H

#include <Misc\GUI\iMenu.h>

namespace Logic
{
    class iMenuControls : public iMenu
    {
    public:
        iMenuControls(iMenu::MenuGroup group);
        void update(int x, int y, float deltaTime);      

    };
}

#endif // !iMENUCONTROLS_H
