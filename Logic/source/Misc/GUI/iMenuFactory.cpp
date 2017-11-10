#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Graphics\include\RenderInfo.h>

using namespace Logic;

#define SKILL_BTN_WIDTH 250
#define SKILL_BTN_HEIGHT 250

iMenu* iMenuFactory::buildMenuStart()
{
    iMenu* menu = newd iMenu(iMenu::Start);
    iMenu::ButtonData btn;

    /* Background */
    menu->addBackground(Resources::Textures::mainMenuButton, 0);

    /* Start Button */
    btn.screenRect = FloatRect(0, 0, 0, 0);
    btn.texRect = FloatRect(0, 1, 0, 1);
    btn.texture = Resources::Textures::mainMenuButton;
    menu->addButton(btn);

    /* Settings Button */
//    btn.callback = ButtonFunction::startSettings;
    btn.screenRect = FloatRect(0, 0, 0, 0);
    btn.texRect = FloatRect(0, 1, 0, 1);
    btn.texture = Resources::Textures::mainMenuButton;
    menu->addButton(btn);

    /* Higscore Button */
//    btn.callback = ButtonFunction::showHighscore;
    btn.screenRect = FloatRect(0, 0, 0, 0);
    btn.texRect = FloatRect(0, 1, 0, 1);
    btn.texture = Resources::Textures::mainMenuButton;
    menu->addButton(btn);

    /* Exit Button */
//    btn.callback = ButtonFunction::quitGame;
    btn.screenRect = FloatRect(0, 0, 0, 0);
    btn.texRect = FloatRect(0, 1, 0, 1);
    btn.texture = Resources::Textures::mainMenuButton;
    menu->addButton(btn);

    return menu;
}

iMenu * iMenuFactory::buildMenuSettings()
{
    return nullptr;
}

iMenu * iMenuFactory::buildMenuSkill()
{
    iMenu* menu = newd iMenu(iMenu::Skill);
    iMenu::ButtonData btn;

    /* Background */
    menu->addBackground(Resources::Textures::mainMenuButton, 1);

    /* Skill One */
//    btn.callback = ButtonFunc::chooseSkill1;
    btn.screenRect = FloatRect(WIN_WIDTH / 2 - WIN_WIDTH / 4 - SKILL_BTN_WIDTH / 2, WIN_HEIGHT / 2 - SKILL_BTN_HEIGHT / 2, SKILL_BTN_WIDTH, SKILL_BTN_HEIGHT);
    btn.texRect = FloatRect(0, 0, 1.f/3.f, 1.f/3.f);
    btn.texture = Resources::Textures::Color_pick_icons;
    menu->addButton(btn);

    /* Skill Two */
//    btn.callback = ButtonFunction::chooseSkill2;
    btn.screenRect = FloatRect(WIN_WIDTH / 2 - SKILL_BTN_WIDTH / 2, WIN_HEIGHT / 2 - SKILL_BTN_HEIGHT / 2, SKILL_BTN_WIDTH, SKILL_BTN_HEIGHT);
    btn.texRect = FloatRect(1.f/3.f, 0, 1.f / 3.f, 1.f / 3.f);
    btn.texture = Resources::Textures::Color_pick_icons;
    menu->addButton(btn);

    /* Skill Three */
//    btn.callback = ButtonFunction::chooseSkill3;
    btn.screenRect = FloatRect(WIN_WIDTH / 2 + WIN_WIDTH / 4 - SKILL_BTN_WIDTH / 2, WIN_HEIGHT / 2 - SKILL_BTN_HEIGHT / 2, SKILL_BTN_WIDTH, SKILL_BTN_HEIGHT);
    btn.texRect = FloatRect(2.f/3.f, 0, 1.f / 3.f, 1.f / 3.f);
    btn.texture = Resources::Textures::Color_pick_icons;
    menu->addButton(btn);

    return menu;
}

iMenu * iMenuFactory::buildMenuCard()
{
    return nullptr;
}

iMenu * iMenuFactory::buildMenuHighscore()
{
    return nullptr;
}

iMenu * iMenuFactory::buildMenuGameover()
{
    return nullptr;
}
