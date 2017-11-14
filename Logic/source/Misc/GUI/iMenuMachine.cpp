#include <Misc\GUI\iMenuMachine.h>
#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Mouse.h>
#include <Keyboard.h>

using namespace Logic;

iMenuMachine::iMenuMachine()
{
    m_factory = newd iMenuFactory;
    m_activeMenu = nullptr;

    Action::Get().SetPointer(this);
}

iMenuMachine::~iMenuMachine()
{
    removeActiveMenu();
}

void iMenuMachine::removeActiveMenu()
{
    if (m_activeMenu)
    {
        delete m_activeMenu;
        m_activeMenu = nullptr;
    }
}

void iMenuMachine::swapMenu(iMenu::MenuGroup group)
{
    removeActiveMenu();

    switch (group)
    {
    case iMenu::MenuGroup::Start:       m_activeMenu = m_factory->buildMenuStart();                       break;
    case iMenu::MenuGroup::Settings:    m_activeMenu = m_factory->buildMenuSettings();                    break;
    case iMenu::MenuGroup::Highscore:   m_activeMenu = m_factory->buildMenuHighscore();                   break;
    case iMenu::MenuGroup::Card:        m_activeMenu = m_factory->buildMenuCard();                        break;
    case iMenu::MenuGroup::Skill:       m_activeMenu = m_factory->buildMenuSkill();                       break;
    case iMenu::MenuGroup::GameOver:    m_activeMenu = m_factory->buildMenuGameover();                    break;
    default: printf("This type of menu is not created yet. Check: %s, Line: %d", __FILE__, __LINE__);   break;
    }
}

void iMenuMachine::update(float deltaTime)
{
    if (m_activeMenu)
    {
        int x = DirectX::Mouse::Get().GetState().x;
        int y = DirectX::Mouse::Get().GetState().y;
        m_activeMenu->update(x, y);
    }
}

void iMenuMachine::render() const
{
    if (m_activeMenu)
        m_activeMenu->render();
}
