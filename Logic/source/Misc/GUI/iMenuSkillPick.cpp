#include <Misc\GUI\iMenuSkillPick.h>

using namespace Logic;

iMenuSkillPick::iMenuSkillPick(iMenu::MenuGroup group) : iMenu(group)
{
    resetSkillPicks();
}

iMenuSkillPick::~iMenuSkillPick() { }

// Callback functions
void iMenuSkillPick::pickOne() { replaceSkill(0); }
void iMenuSkillPick::pickTwo() { replaceSkill(1); }
void iMenuSkillPick::pickThree() { replaceSkill(2); }

// Get functions
int iMenuSkillPick::getPrimarySkill() { return m_selectedSkills.first; }
int iMenuSkillPick::getSecondarySkill() { return m_selectedSkills.second; }

// Resets statss
void iMenuSkillPick::resetSkillPicks()
{
    m_selectedSkills = { -1, -1 };
}

// Returns true if succesful swapping of selected skill, returns false if the selected skill should be reset (if it was already selected, for example)
void iMenuSkillPick::replaceSkill(int id)
{
    printf("Current picks %d, %d\n", m_selectedSkills.first, m_selectedSkills.second);

    // Primary skill pick
    if (m_selectedSkills.first == -1)
    {
        m_selectedSkills.first = id;
    }
    // Secondary skill can be picked and is not the same as the first pick
    else if (m_selectedSkills.second == -1 && m_selectedSkills.first != id)
    {
        m_selectedSkills.second = id;
    }
    // Undo the skillpick
    else
    {
        if (m_selectedSkills.first == id)
            m_selectedSkills.first = -1;

        if (m_selectedSkills.second == id)
            m_selectedSkills.second = -1;
    }
}