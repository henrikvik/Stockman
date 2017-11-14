#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Graphics\include\RenderInfo.h>

using namespace Logic;

#define SKILL_BTN_WIDTH 250
#define SKILL_BTN_HEIGHT 250

iMenuFactory::iMenuFactory()
{
    FileLoader::singleton().loadStructsFromFile(buttonFile, "Button");
}

iMenuFactory::~iMenuFactory() { }

iMenu* iMenuFactory::buildMenuStart()
{
    iMenu* menu = newd iMenu(iMenu::Start);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::mainmenupicture, 0.1f);
    menu->addButton(buildButton("MenuStartGame"));
    menu->addButton(buildButton("MenuStartSettings"));
    menu->addButton(buildButton("MenuStartHighscore"));
    menu->addButton(buildButton("MenuQuitGame"));

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

    menu->addBackground(Resources::Textures::mainMenuButton, 1);
    menu->addButton(buildButton("SkillPickButton1"));
    menu->addButton(buildButton("SkillPickButton2"));
    menu->addButton(buildButton("SkillPickButton3"));

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

iMenu::ButtonData iMenuFactory::buildButton(std::string name)
{
    iMenu::ButtonData btn;
    for (auto const& button : buttonFile)
    {
        //If it is a button add it into its map
        if (button.strings.find("buttonName") != button.strings.end() &&
            button.strings.at("buttonName") == name)
        {
            btn.screenRect.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("xPos") / WIN_WIDTH, button.floats.at("yPos") / WIN_WIDTH);
            btn.screenRect.bottomRight = DirectX::SimpleMath::Vector2((button.floats.at("width") + button.floats.at("xPos")) / WIN_WIDTH, (button.floats.at("height") + button.floats.at("yPos")) / WIN_WIDTH);
            btn.texRectNormal.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("xTexStart"), button.floats.at("yTexStart"));
            btn.texRectNormal.bottomRight = DirectX::SimpleMath::Vector2(button.floats.at("xTexEnd"), button.floats.at("yTexEnd"));
            btn.texRectHover.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("xTexStart"), button.floats.at("yTexStart") + button.floats.at("activeOffset"));
            btn.texRectHover.bottomRight = DirectX::SimpleMath::Vector2(button.floats.at("xTexEnd"), button.floats.at("yTexEnd") + button.floats.at("activeOffset"));
            btn.texRectActive = btn.texRectHover;
            btn.texture = Resources::Textures::mainMenuText; // Doesn't work right now Resources::Textures::Files(button.ints.at("texture"));
        }
    }
    return btn;
}
