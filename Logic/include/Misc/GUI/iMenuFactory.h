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
        iMenuHighscore*     buildMenuHighscore();
        iMenuHighscore*     buildMenuHighscoreGameOver();
        iMenu*              buildMenuGameover();
        iMenu*              buildMenuPause();
        iMenu*              buildMenuGameWon();
        iMenuLoading*       buildMenuLoading();

    private:
        std::vector<FileLoader::LoadedStruct> buttonFile;
        iMenu::ButtonData buildButton(std::string name, std::function<void(void)>);
    };
}

#endif // !IMENUFACTORY_H
