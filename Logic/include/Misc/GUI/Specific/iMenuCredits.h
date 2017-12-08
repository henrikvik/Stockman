#ifndef MENU_CREDITS_H
#define MENU_CREDITS_H

#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderInfo.h>
#include <Misc\Credits.h>

namespace Logic
{
    class iMenuCredits : public iMenu
    {
    private:
        Logic::Credits credits;
    public:
        iMenuCredits(iMenu::MenuGroup group);
        ~iMenuCredits();

        void update(int x, int y, float deltaTime);
        void render() const;
    };
}

#endif