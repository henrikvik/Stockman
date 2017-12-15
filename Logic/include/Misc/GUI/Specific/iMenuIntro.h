#ifndef iMENUINTRO_H
#define iMENUINTRO_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>

namespace Logic
{
    class iMenuIntro : public iMenu
    {
    public:
        iMenuIntro(iMenu::MenuGroup group);
        void update(int x, int y, float deltaTime);
    };
}

#endif // !iMENUINTRO_H