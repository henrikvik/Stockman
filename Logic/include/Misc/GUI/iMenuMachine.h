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
        void update(float deltaTime);
        void render() const;

        iMenu* getActiveMenu()              { return m_activeMenu;      }
        iMenu::MenuGroup getType() const    { return m_currentMenuType; }

        void setDeathPosition(DirectX::SimpleMath::Vector3 position) { m_deathPosition = position; }

    private:
        DirectX::SimpleMath::Vector3 m_deathPosition; // testing a thing
        iMenu::MenuGroup m_queuedMenuType;
        iMenu::MenuGroup m_currentMenuType;
        iMenuFactory* m_factory;
        iMenu* m_activeMenu;

        // For internal use only
        void removeActiveMenu();
        bool wantsToSwap();
        void swapMenu();
    };
}

#endif // !IMENUMACHINE_H
