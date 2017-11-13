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

        void removeActiveMenu();
        void swapMenu(iMenu::MenuGroup group);

        void update(float deltaTime);
        void render() const;

        iMenu* getActiveMenu() { return m_activeMenu; }

    private:
        iMenuFactory* m_factory;
        iMenu* m_activeMenu;
    };
}

#endif // !IMENUMACHINE_H
