#include <Misc\GUI\iMenuSkillPick.h>
#include <Graphics\include\RenderQueue.h>

#include <Keyboard.h>
#include <Misc\GUI\iMenuAction.h>

using namespace Logic;

iMenuSkillPick::iMenuSkillPick(iMenu::MenuGroup group) : iMenu(group)
{
    resetSkillPicks();

    // Setup of the textrenderinfo object
    m_textRenderInfo.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfo.font = Resources::Fonts::nordic;
    m_textRenderInfo.position = DirectX::SimpleMath::Vector2(0.01 * WIN_WIDTH, 0.28 * WIN_HEIGHT);
    m_skillpointsStr = std::to_wstring(m_skillPoints) + (const wchar_t*)L" Skillpoints";
    m_textRenderInfo.text = m_skillpointsStr.c_str();

    m_grapplingHook.alpha = 1.f;
    m_grapplingHook.isMoveable = true;
    m_grapplingHook.texture = Resources::Textures::TextSkillPick;
    m_grapplingHook.screenRect = FloatRect({
        700.0f / WIN_WIDTH,
        160.0f / WIN_HEIGHT,
        400.0f / WIN_WIDTH,
        300.0f / WIN_HEIGHT,
    });
    m_grapplingHook.textureRect = FloatRect({ 0.0f, 0.0f }, { 0.37109375f, 0.3125f });

    m_spriteRenderInfo1.alpha = 1.f;
    m_spriteRenderInfo1.isMoveable = true;
    m_spriteRenderInfo1.texture = Resources::Textures::TextSkillPick;
    m_spriteRenderInfo1.screenRect = FloatRect({
        700.0f / WIN_WIDTH,
        160.0f / WIN_HEIGHT,
        400.0f / WIN_WIDTH,
        300.0f / WIN_HEIGHT,
    });
    m_spriteRenderInfo1.textureRect = FloatRect({ 0.0f, 0.3125f }, { 0.37109375f, 0.64453125f });

    m_spriteRenderInfo2.alpha = 1.f;
    m_spriteRenderInfo2.isMoveable = true;
    m_spriteRenderInfo2.texture = Resources::Textures::TextSkillPick;
    m_spriteRenderInfo2.screenRect = FloatRect({
        700.0f / WIN_WIDTH,
        160.0f / WIN_HEIGHT,
        400.0f / WIN_WIDTH,
        300.0f / WIN_HEIGHT,
    });
    m_spriteRenderInfo2.textureRect = FloatRect({ 0.0f, 0.64453125f }, { 0.37109375f, 1.0f });

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
    m_skillPoints = 0;
    if (m_selectedSkills.first == -1) m_skillPoints++;
    if (m_selectedSkills.second == -1) m_skillPoints++;

    // Update the text.
    m_skillpointsStr = std::to_wstring(m_skillPoints) + (const wchar_t*)L" Skillpoints";
    m_textRenderInfo.text = m_skillpointsStr.c_str();

    for (size_t i = 0; i < 3; i++)
    {

         m_buttons.at(i).setState(Button::INACTIVE);

    }

    if (m_selectedSkills.first != -1)
    {
        m_buttons.at(m_selectedSkills.first).setState(Button::ACTIVE);
    }

    if (m_selectedSkills.second != -1)
    {
        m_buttons.at(m_selectedSkills.second).setState(Button::ACTIVE);
    }
}

// Draws the available skill picks as a text
void iMenuSkillPick::render() const
{
    iMenu::render();
    QueueRender(m_textRenderInfo);
    //QueueRender(m_grapplingHook);
    QueueRender(m_spriteRenderInfo1);
   // QueueRender(m_spriteRenderInfo2);
}

// Callback functions
void iMenuSkillPick::pickOne()      { replaceSkill(0); }
void iMenuSkillPick::pickTwo()      { replaceSkill(1); }
void iMenuSkillPick::pickThree()    { replaceSkill(2); }

// Get functions
int iMenuSkillPick::getPrimarySkill()   { return m_selectedSkills.first;    }
int iMenuSkillPick::getSecondarySkill() { return m_selectedSkills.second;   }