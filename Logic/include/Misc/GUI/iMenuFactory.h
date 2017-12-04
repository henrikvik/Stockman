#ifndef IMENUFACTORY_H
#define IMENUFACTORY_H

#include <Misc\GUI\iMenu.h>
#include <Misc\NonCopyable.h>
#include <Misc\FileLoader.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\Specific\iMenuSkillPick.h>
#include <Misc\GUI\Specific\iMenuIntro.h>
#include <Misc\GUI\Specific\iMenuLoading.h>
#include <Misc\GUI\Specific\iMenuHighscore.h>
#include <Misc\GUI\Specific\iMenuFirstTime.h>
#include <Misc\GUI\Specific\iMenuCards.h>
#include <Misc\GUI\Specific\iMenuControls.h>
#include <Misc\GUI\Specific\iMenuSettings.h>
#include <Misc\GUI\Specific\iMenuGameOver.h>
#include <Misc\GUI\Specific\iMenuCinematic.h>

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
        iMenuSettings*      buildMenuPauseSettings();
        iMenuSkillPick*     buildMenuSkill();
        iMenuCards*         buildMenuCard();
        iMenuHighscore*     buildMenuHighscore();
        iMenuHighscore*     buildMenuHighscoreGameOver();
        iMenuGameOver*      buildMenuGameover();
        iMenu*              buildMenuPause();
        iMenu*              buildMenuGameWon();
        iMenuControls*      buildMenuControls();
        iMenuLoadingPre*    buildMenuLoadingPre();
        iMenuLoadingPost*   buildMenuLoadingPost();
        iMenuCinematic*     buildMenuCinematic();

    private:
        std::vector<FileLoader::LoadedStruct> buttonFile;
        std::vector<FileLoader::LoadedStruct> sliderFile;
        iMenu::ButtonData buildButton(std::string name, std::function<void(void)>);
        iMenu::SliderData buildSlider(std::string name, float* value, float minValue, float maxValue, float delimiter);
    };
}

#endif // !IMENUFACTORY_H
