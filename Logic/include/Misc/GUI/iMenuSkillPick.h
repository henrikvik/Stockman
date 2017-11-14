#ifndef iMENUSKILLPICK_H
#define iMENUSKILLPICK_H

#include <Misc\GUI\iMenu.h>

namespace Logic
{
    class iMenuSkillPick : public iMenu
    {
    public:
        iMenuSkillPick(iMenu::MenuGroup group);
        ~iMenuSkillPick();

        void pickOne();
        void pickTwo();
        void pickThree();

        void resetSkillPicks();
        void replaceSkill(int id);

        int getPrimarySkill();
        int getSecondarySkill();

    private:
        std::pair<int, int> m_selectedSkills;
    };
}

#endif // !iMENUSKILLPICK_H