#ifndef IMENUMACHINE_H
#define IMENUMACHINE_H

#include <Misc\NonCopyable.h>
#include <Misc\GUI\iMenu.h>

namespace Logic
{
    class iMenuFactory;
    class iMenuMachine : public NonCopyable
    {
    public:
        iMenuMachine();
        ~iMenuMachine();

        void queueMenu(iMenu::MenuGroup group);
        void removeActiveMenu();
        void update(float deltaTime);
        void render() const;

        iMenu* getActiveMenu() { return m_activeMenu; }
        bool wantsToSwap();

    private:
        iMenu::MenuGroup m_queuedMenuType;
        iMenu::MenuGroup m_currentMenuType;
        iMenuFactory* m_factory;
        iMenu* m_activeMenu;

        void swapMenu();
    };
}

#endif // !IMENUMACHINE_H
