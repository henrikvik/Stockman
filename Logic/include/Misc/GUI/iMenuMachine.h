#ifndef IMENUMACHINE_H
#define IMENUMACHINE_H

#include <Misc\NonCopyable.h>
#include <Misc\GUI\iMenu.h>

// Makes it more readable
using namespace DirectX::SimpleMath; 

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
        void updateCamera(float deltaTime);
        void render() const;

        iMenu* getActiveMenu()              { return m_activeMenu;      }
        iMenu::MenuGroup getType() const    { return m_currentMenuType; }

        void startDeathAnimation(Vector3 position, Vector3 forward);

    private:

        // Camera
        Vector3 m_deathPosition;
        Vector3 m_deathForward;
        bool m_resetDeathPositionAndForward;
        
        // Menu's
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
