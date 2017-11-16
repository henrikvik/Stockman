#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Graphics\include\RenderInfo.h>

using namespace Logic;

iMenuFactory::iMenuFactory()
{
    // Loads all buttons from Button.lw file (Engine/Resources/Button.lw)
    FileLoader::singleton().loadStructsFromFile(buttonFile, "Button");
}

iMenuFactory::~iMenuFactory() { }

const std::map<int, Resources::Textures::Files> LookUp =
{
    { 0, Resources::Textures::Mainmenutext },               // - The four selections on the starting screen
    { 1, Resources::Textures::gameOverMenuButtons },        // OLD - Remove later
    { 2, Resources::Textures::SettingsMenuButtons },        // OLD - Remove later
    { 3, Resources::Textures::Skillpicksheet },             // - Skill pick buttons, and continue button
    { 4, Resources::Textures::Backbutton },                 // OLD - Remove later
    { 5, Resources::Textures::Highscoretext }               // - Same as MainMenuText but with the "Back Button", that we want
};

iMenuIntro* iMenuFactory::buildMenuIntro()
{
    iMenuIntro* menu = newd iMenuIntro(iMenu::Intro);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::IntroScreen, 1.f);

    return menu;
}

iMenu* iMenuFactory::buildMenuStart()
{
    iMenu* menu = newd iMenu(iMenu::Start);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::MainmenuClean, 1.f);
    menu->addButton(buildButton("MenuStartGame",        ButtonFunction::startGame));
    menu->addButton(buildButton("MenuStartSettings",    ButtonFunction::showHighscore));
    menu->addButton(buildButton("MenuStartHighscore",   ButtonFunction::startSettings));
    menu->addButton(buildButton("MenuQuitGame",         ButtonFunction::quitGame));

    return menu;
}

iMenu * iMenuFactory::buildMenuSettings()
{
    iMenu* menu = newd iMenu(iMenu::Settings);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::Settings, 1.f);
    menu->addButton(buildButton("MenuBackGame", ButtonFunction::startMainMenu));

    //menu->addButton(buildButton("MenuSettingsWriting", ButtonFunction::writing));
    //menu->addButton(buildButton("MenuSettingsStartMenu", ButtonFunction::startMainMenu));
    //menu->addButton(buildButton("MenuSettingsSoundMasterMinus", ButtonFunction::minusMaster));
    //menu->addButton(buildButton("MenuSettingsSoundMasterPlus", ButtonFunction::plusMaster));
    //menu->addButton(buildButton("MenuSettingsSoundSFXMinus", ButtonFunction::minusSFX));
    //menu->addButton(buildButton("MenuSettingsSoundSFXPlus", ButtonFunction::plusSFX));
    //menu->addButton(buildButton("MenuSettingsMuteUnmute", ButtonFunction::muteUnmute));
    //menu->addButton(buildButton("MenuSettingsControlsMouseSenseMinus", ButtonFunction::minusSense));
    //menu->addButton(buildButton("MenuSettingsControlsMouseSensePlus", ButtonFunction::plusSense));
    //menu->addButton(buildButton("MenuSettingsVideoFOVMinus", ButtonFunction::minusFOV));
    //menu->addButton(buildButton("MenuSettingsVideoFOVPlus", ButtonFunction::plusFOV));
    //menu->addButton(buildButton("MenuSettingsVideoWindowed", ButtonFunction::windowed));

    return menu;
}

iMenuSkillPick* iMenuFactory::buildMenuSkill()
{
    iMenuSkillPick* menu = newd iMenuSkillPick(iMenu::Skill);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::Skillpickbackground, 1.f);
    menu->addButton(buildButton("SkillPickButton1", std::bind(&iMenuSkillPick::pickOne, menu)));
    menu->addButton(buildButton("SkillPickButton2", std::bind(&iMenuSkillPick::pickTwo, menu)));
    menu->addButton(buildButton("SkillPickButton3", std::bind(&iMenuSkillPick::pickThree, menu)));
    menu->addButton(buildButton("Continue", ButtonFunction::confirmSkillPicks));

    return menu;
}

iMenu * iMenuFactory::buildMenuCard()
{
    iMenu* menu = newd iMenu(iMenu::CardSelect);
    iMenu::ButtonData btn;

    menu->addButton(buildButton("CardUpgradeChoice1", ButtonFunction::chooseUpgrade1));
    menu->addButton(buildButton("CardUpgradeChoice2", ButtonFunction::chooseUpgrade2));
    menu->addButton(buildButton("CardUpgradeChoice3", ButtonFunction::chooseUpgrade3));

    return menu;
}

iMenu * iMenuFactory::buildMenuHighscore()
{
    iMenu* menu = newd iMenu(iMenu::Highscore);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::Highscore, 1.f);
    menu->addButton(buildButton("MenuBackGame", ButtonFunction::startMainMenu));
    
    //menu->addButton(buildButton("HighscoreStartMenu", ButtonFunction::startMainMenu));

    return menu;
}

iMenu * iMenuFactory::buildMenuGameover()
{
    iMenu* menu = newd iMenu(iMenu::GameOver);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::MainmenuClean, 0.70f);
    menu->addButton(buildButton("MenuStartGame", ButtonFunction::playAgain));
    menu->addButton(buildButton("MenuQuitGame", ButtonFunction::goBackToMainMenu));

    return menu;
}

// Building function, for internal use only
iMenu::ButtonData iMenuFactory::buildButton(std::string name, std::function<void(void)> func)
{
    iMenu::ButtonData btn;
    for (auto const& button : buttonFile)
    {
        if (button.strings.find("buttonName") != button.strings.end() &&
            button.strings.at("buttonName") == name)
        {
            btn.callback = func;
            btn.screenRect.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("xPos") / WIN_WIDTH, button.floats.at("yPos") / WIN_WIDTH);
            btn.screenRect.bottomRight = DirectX::SimpleMath::Vector2((button.floats.at("width") + button.floats.at("xPos")) / WIN_WIDTH, (button.floats.at("height") + button.floats.at("yPos")) / WIN_WIDTH);
            btn.texRectNormal.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("xTexStart"), button.floats.at("yTexStart"));
            btn.texRectNormal.bottomRight = DirectX::SimpleMath::Vector2(button.floats.at("xTexEnd"), button.floats.at("yTexEnd"));
            btn.texRectHover.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("hoverXTexStart"), button.floats.at("hoverYTexStart"));
            btn.texRectHover.bottomRight = DirectX::SimpleMath::Vector2(button.floats.at("hoverXTexEnd"), button.floats.at("hoverYTexEnd"));
            btn.texRectActive.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("activeXTexStart"), button.floats.at("activeYTexStart"));
            btn.texRectActive.bottomRight = DirectX::SimpleMath::Vector2(button.floats.at("activeXTexEnd"), button.floats.at("activeYTexEnd"));
           
            /*btn.texRectHover.topLeft = DirectX::SimpleMath::Vector2(button.floats.at("xTexStart"), button.floats.at("yTexStart") + button.floats.at("activeOffset"));
            btn.texRectHover.bottomRight = DirectX::SimpleMath::Vector2(button.floats.at("xTexEnd"), button.floats.at("yTexEnd") + button.floats.at("activeOffset"));
            btn.texRectActive = btn.texRectHover;*/
            btn.texture = LookUp.at(button.ints.at("texture")); 
        }
    }
    return btn;
}