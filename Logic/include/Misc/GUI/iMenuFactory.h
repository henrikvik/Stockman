#ifndef IMENUFACTORY_H
#define IMENUFACTORY_H

#include <Misc\GUI\iMenu.h>
#include <Misc\NonCopyable.h>
#include <Misc\FileLoader.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuSkillPick.h>
#include <Misc\GUI\iMenuIntro.h>
#include <Misc\GUI\iMenuLoading.h>
#include <Misc\GUI\iMenuHighscore.h>
#include <Misc\GUI\iMenuFirstTime.h>
#include <Misc\GUI\iMenuCards.h>
#include <Misc\GUI\iMenuSettings.h>

namespace Logic
{
    class iMenuFactory : public NonCopyable
    {
    public:
        iMenuFactory();
        ~iMenuFactory();

        iMenuIntro*         buildMenuIntro();
        iMenuFirstTime*     buildMenuFirstTime();
        iMenu*              buildMenuStart();
        iMenuSettings*      buildMenuSettings();
        iMenuSkillPick*     buildMenuSkill();
        iMenuCards*         buildMenuCard();
        iMenuHighscore*     buildMenuHighscore();
        iMenuHighscore*     buildMenuHighscoreGameOver();
        iMenu*              buildMenuGameover();
        iMenu*              buildMenuPause();
        iMenu*              buildMenuGameWon();
        iMenuLoadingPre*    buildMenuLoadingPre();
        iMenuLoadingPost*   buildMenuLoadingPost();

    private:
        std::vector<FileLoader::LoadedStruct> buttonFile;
        std::vector<FileLoader::LoadedStruct> sliderFile;
        iMenu::ButtonData buildButton(std::string name, std::function<void(void)>);
        iMenu::SliderData buildSlider(std::string name, float* value, float minValue, float maxValue, float delimiter);
    };
}

#endif // !IMENUFACTORY_H
