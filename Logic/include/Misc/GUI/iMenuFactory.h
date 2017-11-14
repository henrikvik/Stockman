#ifndef IMENUFACTORY_H
#define IMENUFACTORY_H

#include <Misc\GUI\iMenu.h>
#include <Misc\NonCopyable.h>
#include <Misc\FileLoader.h>
#include <Misc\GUI\iMenuAction.h>

namespace Logic
{
    class iMenuFactory : public NonCopyable
    {
    public:
        iMenuFactory();
        ~iMenuFactory();

        iMenu* buildMenuStart();
        iMenu* buildMenuSettings();
        iMenu* buildMenuSkill();
        iMenu* buildMenuCard();
        iMenu* buildMenuHighscore();
        iMenu* buildMenuGameover();

    private:
        std::vector<FileLoader::LoadedStruct> buttonFile;
        iMenu::ButtonData buildButton(std::string name, ButtonFunc func);
    };
}

#endif // !IMENUFACTORY_H
