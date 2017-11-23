#ifndef IMENUFACTORY_H
#define IMENUFACTORY_H

#include <Misc\GUI\iMenu.h>
#include <Misc\NonCopyable.h>
#include <Misc\FileLoader.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuSkillPick.h>
#include <Misc\GUI\iMenuIntro.h>
#include <Misc\GUI\iMenuLoading.h>

namespace Logic
{
    class iMenuFactory : public NonCopyable
    {
    public:
        iMenuFactory();
        ~iMenuFactory();

        iMenuIntro*         buildMenuIntro();
        iMenu*              buildMenuStart();
        iMenu*              buildMenuSettings();
        iMenuSkillPick*     buildMenuSkill();
        iMenu*              buildMenuCard();
        iMenu*              buildMenuHighscore();
        iMenu*              buildMenuGameover();
        iMenu*              buildMenuPause();
        iMenu*              buildMenuGameWon();
        iMenuLoading*       buildMenuLoading();

    private:
        std::vector<FileLoader::LoadedStruct> buttonFile;
        std::vector<FileLoader::LoadedStruct> sliderFile;
        iMenu::ButtonData buildButton(std::string name, std::function<void(void)>);
        iMenu::SliderData buildSlider(std::string name, float* value, float minValue, float maxValue);
    };
}

#endif // !IMENUFACTORY_H
