#include <Misc\GUI\iMenuSkillPick.h>
#include <Graphics\include\RenderQueue.h>
#include <Misc\StringConverter.h>
#include <Keyboard.h>
#include <Misc\GUI\iMenuAction.h>

using namespace Logic;

iMenuSkillPick::iMenuSkillPick(iMenu::MenuGroup group) : iMenu(group)
{
    resetSkillPicks();

    // Setup of the textrenderinfo object
    m_textRenderInfo.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfo.font = Resources::Fonts::KG18;
    m_textRenderInfo.position = DirectX::SimpleMath::Vector2(0.01 * WIN_WIDTH, 0.28 * WIN_HEIGHT);
    m_skillpointsStr = std::to_wstring(m_skillPoints) + (const wchar_t*)L" Skillpoints";
    m_textRenderInfo.text = m_skillpointsStr.c_str();
}

iMenuSkillPick::~iMenuSkillPick() { }

void iMenuSkillPick::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    // Confirm with space
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Space) && !m_isFading && !m_safeToRemove)
        ButtonFunction::confirmSkillPicks();
}

// Resets stats
void iMenuSkillPick::resetSkillPicks()
{
    m_skillPoints = 2;
    m_selectedSkills = { -1, -1 };
}

// Returns true if succesful swapping of selected skill, returns false if the selected skill should be reset (if it was already selected, for example)
void iMenuSkillPick::replaceSkill(int id)
{
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

    // Count skillpoints
    m_skillPoints = 2;
    if (m_selectedSkills.first != -1) m_skillPoints--;
    if (m_selectedSkills.second != -1) m_skillPoints--;

    // Update the text.
    m_skillpointsStr = std::to_wstring(m_skillPoints) + (const wchar_t*)L" Skillpoints";
    m_textRenderInfo.text = m_skillpointsStr.c_str();
}

// Draws the available skill picks as a text
void iMenuSkillPick::render() const
{
    iMenu::render();
    QueueRender(m_textRenderInfo);
}

// Callback functions
void iMenuSkillPick::pickOne()      { replaceSkill(0); }
void iMenuSkillPick::pickTwo()      { replaceSkill(1); }
void iMenuSkillPick::pickThree()    { replaceSkill(2); }

// Get functions
int iMenuSkillPick::getPrimarySkill()   { return m_selectedSkills.first;    }
int iMenuSkillPick::getSecondarySkill() { return m_selectedSkills.second;   }