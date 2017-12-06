#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Graphics\include\RenderInfo.h>
#include <Engine\Settings.h>
#include <Misc\GUI\iMenuFX.h>

using namespace Logic;

iMenuFactory::iMenuFactory()
{
    // Loads all buttons from Button.lw file (Engine/Resources/Button.lw)
    FileLoader::singleton().loadStructsFromFile(buttonFile, "Button");

    FileLoader::singleton().loadStructsFromFile(sliderFile, "Slider");
}

iMenuFactory::~iMenuFactory() { }

const std::map<int, Resources::Textures::Files> LookUp =
{
    { 0, Resources::Textures::Mainmenutext },               // - The four selections on the starting screen
    { 1, Resources::Textures::CardBackground },                  // card backgrounds
    { 2, Resources::Textures::Retrytextsheet },        // OLD - Remove later
    { 3, Resources::Textures::Skillpicksheet },             // - Skill pick buttons, and continue button
    //{ 4, Resources::Textures::Backbutton },                 // OLD - Remove later
    { 5, Resources::Textures::Highscoretext },              // - Same as MainMenuText but with the "Back Button", that we want
   // { 6, Resources::Textures::Pausetext },                   // - Same as MainMenuText but with the "Return to Menu", that we want
    { 7, Resources::Textures::SettingsButtons}
};

iMenuIntro* iMenuFactory::buildMenuIntro()
{
    iMenuIntro* menu = newd iMenuIntro(iMenu::Intro);
    menu->addBackground(Resources::Textures::IntroScreen, 1.f);
    return menu;
}

iMenuFirstTime * iMenuFactory::buildMenuFirstTime()
{
    iMenuFirstTime* menu = newd iMenuFirstTime(iMenu::FirstTime);
    menu->addBackground(Resources::Textures::MainmenuClean, 1.f);
    return menu;
}

iMenu* iMenuFactory::buildMenuStart()
{
    iMenu* menu = newd iMenu(iMenu::Start);

    menu->addBackground(Resources::Textures::MainmenuClean, 1.f);
    menu->addButton(buildButton("MenuStartGame",        ButtonFunction::startGame));
    menu->addButton(buildButton("MenuStartGameHeroic",  ButtonFunction::startGameHeroic));
    menu->addButton(buildButton("MenuStartSettings",    ButtonFunction::showHighscore));
    menu->addButton(buildButton("MenuStartHighscore",   ButtonFunction::startSettings));
    menu->addButton(buildButton("MenuQuitGame",         ButtonFunction::quitGame));

    return menu;
}

iMenuSettings* iMenuFactory::buildMenuSettings()
{
    iMenuSettings* menu = newd iMenuSettings(iMenu::SettingsStart);

    menu->addBackground(Resources::Textures::Settings, 1.f);
    menu->addButton(buildButton("MenuQuitGame", ButtonFunction::startMainMenu));
    Settings& setting = Settings::getInstance();
    menu->addSlider(buildSlider("MouseSlider", setting.getMouseSensePTR(), 0.001f, 0.2f, 0.001f));
    menu->addSlider(buildSlider("MasterSlider", setting.getMasterSoundPTR(), 0.0f, 1.0f, 0.01f));
    menu->addSlider(buildSlider("MusicSlider", setting.getMusicPTR(), 0.0f, 1.0f, 0.01f));
    menu->addSlider(buildSlider("AmbienceSlider", setting.getAmbiencePTR(), 0.0f, 1.0f, 0.01f));
    menu->addSlider(buildSlider("SFXSlider", setting.getSFXPTR(), 0.0f, 1.0f, 0.01f));
    menu->addSlider(buildSlider("FOVSlider", setting.getFOVPTR(), 60.0f, 120.0f, 1.0f));
    menu->addButton(buildButton("MenuSettingsVideoWindowedLeft", ButtonFunction::windowed));
    menu->addButton(buildButton("MenuSettingsVideoWindowedRight", ButtonFunction::windowed));
    menu->addButton(buildButton("MenuSettingsVideoDOFLeft", ButtonFunction::DOF));
    menu->addButton(buildButton("MenuSettingsVideoDOFRight", ButtonFunction::DOF));
    menu->addButton(buildButton("MenuSettingsVideoSSAOLeft", ButtonFunction::SSAO));
    menu->addButton(buildButton("MenuSettingsVideoSSAORight", ButtonFunction::SSAO));
    menu->addButton(buildButton("MenuSettingsVideoFogLeft", ButtonFunction::fog));
    menu->addButton(buildButton("MenuSettingsVideoFogRight", ButtonFunction::fog));

    return menu;
}

iMenuSettings * iMenuFactory::buildMenuPauseSettings()
{
    iMenuSettings* menu = buildMenuSettings();
    
    menu->setGroup(iMenu::MenuGroup::SettingsPause);
    menu->removeButtons();
    menu->addButton(buildButton("MenuQuitGame", ButtonFunction::pause));

    return menu;
}

iMenuSkillPick* iMenuFactory::buildMenuSkill()
{
    iMenuSkillPick* menu = newd iMenuSkillPick(iMenu::Skill);
    menu->addEffect((iMenuFX*)(newd iMenuFX_Dust()));
    menu->addBackground(Resources::Textures::Skillpickbackground, 1.f);
    menu->addButton(buildButton("SkillPickButton1", std::bind(&iMenuSkillPick::pickOne, menu)));
    menu->addButton(buildButton("SkillPickButton2", std::bind(&iMenuSkillPick::pickTwo, menu)));
    menu->addButton(buildButton("SkillPickButton3", std::bind(&iMenuSkillPick::pickThree, menu)));
    menu->addButton(buildButton("Continue", ButtonFunction::confirmSkillPicks));

    return menu;
}

iMenuCards * iMenuFactory::buildMenuCard()
{
    iMenuCards* menu = newd iMenuCards(iMenu::CardSelect);
    menu->addButton(buildButton("CardUpgradeChoice1", ButtonFunction::chooseUpgrade1));
    menu->addButton(buildButton("CardUpgradeChoice2", ButtonFunction::chooseUpgrade2));
    menu->addButton(buildButton("CardUpgradeChoice3", ButtonFunction::chooseUpgrade3));
    return menu;
}

iMenu * iMenuFactory::buildMenuGameWon()
{
    iMenu* menu = newd iMenu(iMenu::GameWon);
    menu->addBackground(Resources::Textures::Gameover, 1.f);
    return menu;
}

iMenuControls * iMenuFactory::buildMenuControls()
{
    iMenuControls* menu = newd iMenuControls(iMenu::Controls);
    menu->addEffect(newd iMenuFX_Dust());
    menu->addBackground(Resources::Textures::controlscheme, 1.f);
    return menu;
}

iMenuLoadingPre* iMenuFactory::buildMenuLoadingPre()
{
    iMenuLoadingPre* menu = newd iMenuLoadingPre(iMenu::LoadingPre);
    menu->addEffect(newd iMenuFX_Dust());
    menu->addBackground(Resources::Textures::Loadingscreen, 1.f);
    return menu;
}

iMenuLoadingPost* iMenuFactory::buildMenuLoadingPost()
{
    iMenuLoadingPost* menu = newd iMenuLoadingPost(iMenu::LoadingPost);
    menu->addEffect(newd iMenuFX_Dust());
    menu->addBackground(Resources::Textures::Loadingscreen, 1.f);
    return menu;
}

iMenuHighscore * iMenuFactory::buildMenuHighscore()
{
    iMenuHighscore* menu = newd iMenuHighscore(iMenu::HighscoreStartMenu);
    menu->addBackground(Resources::Textures::Highscore, 1.f);
    menu->addButton(buildButton("MenuBackGame", ButtonFunction::startMainMenu));
    return menu;
}

iMenuHighscore * iMenuFactory::buildMenuHighscoreGameOver()
{
    iMenuHighscore* menu = newd iMenuHighscore(iMenu::HighscoreGameOver);
    menu->addBackground(Resources::Textures::Highscore, 1.f);
    menu->addButton(buildButton("MenuBackGame", ButtonFunction::goToGameOver));
    return menu;
}

iMenuGameOver * iMenuFactory::buildMenuGameover()
{
    iMenuGameOver* menu = newd iMenuGameOver(iMenu::GameOver);
    iMenu::ButtonData btn;

    menu->addBackground(Resources::Textures::Gameover, 1.0f);

    btn = buildButton("Retry", ButtonFunction::playAgain);
    btn.move(DirectX::SimpleMath::Vector2(0.333, 0.10));
    menu->addButton(btn);

    btn = buildButton("MenuStartSettings", ButtonFunction::goToGameOverHighscore);
    btn.move(DirectX::SimpleMath::Vector2(0.333, 0.09));
    menu->addButton(btn);

    btn = buildButton("MenuQuitGame", ButtonFunction::goBackToMainMenu);
    btn.move(DirectX::SimpleMath::Vector2(0.333, 0.05));
    menu->addButton(btn);

    return menu;
}

iMenuCinematic * iMenuFactory::buildMenuCinematic()
{
    iMenuCinematic* menu = newd iMenuCinematic(iMenu::Cinematic);
    return menu;
}

iMenu * iMenuFactory::buildMenuPause()
{
    iMenu* menu = newd iMenu(iMenu::Pause);
    menu->addBackground(Resources::Textures::MainmenuClean, 1.f);
    menu->addButton(buildButton("MenuStartGame", ButtonFunction::unpause));
    menu->addButton(buildButton("Restart", ButtonFunction::playAgain));
    menu->addButton(buildButton("MenuStartHighscore", ButtonFunction::startSettingsPause));
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
            btn.texture = LookUp.at(button.ints.at("texture")); 
        }
    }
    return btn;
}

iMenu::SliderData iMenuFactory::buildSlider(std::string name, float* value, float minValue, float maxValue, float delimiter)
{
    iMenu::SliderData sld;
    for (auto const& slider : sliderFile)
    {
        if (slider.strings.find("sliderName") != slider.strings.end() &&
            slider.strings.at("sliderName") == name)
        {
            sld.name = name;
            sld.screenRect.topLeft = DirectX::SimpleMath::Vector2(slider.floats.at("xPos") / WIN_WIDTH, slider.floats.at("yPos") / WIN_WIDTH);
            sld.screenRect.bottomRight = DirectX::SimpleMath::Vector2((slider.floats.at("width") + slider.floats.at("xPos")) / WIN_WIDTH, (slider.floats.at("height") + slider.floats.at("yPos")) / WIN_WIDTH);
            sld.texRectNormal.topLeft = DirectX::SimpleMath::Vector2(slider.floats.at("xTexStart"), slider.floats.at("yTexStart"));
            sld.texRectNormal.bottomRight = DirectX::SimpleMath::Vector2(slider.floats.at("xTexEnd"), slider.floats.at("yTexEnd"));
            sld.texRectHover.topLeft = DirectX::SimpleMath::Vector2(slider.floats.at("hoverXTexStart"), slider.floats.at("hoverYTexStart"));
            sld.texRectHover.bottomRight = DirectX::SimpleMath::Vector2(slider.floats.at("hoverXTexEnd"), slider.floats.at("hoverYTexEnd"));
            sld.texRectActive.topLeft = DirectX::SimpleMath::Vector2(slider.floats.at("activeXTexStart"), slider.floats.at("activeYTexStart"));
            sld.texRectActive.bottomRight = DirectX::SimpleMath::Vector2(slider.floats.at("activeXTexEnd"), slider.floats.at("activeYTexEnd"));
            sld.texture = LookUp.at(slider.ints.at("texture"));
            sld.min = slider.floats.at("min");
            sld.max = slider.floats.at("max");
            sld.value = value;
            sld.minValue = minValue;
            sld.maxValue = maxValue;
            sld.delimeter = delimiter;
        }
    }
    return sld;
}