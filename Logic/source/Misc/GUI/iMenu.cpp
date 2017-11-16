#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderQueue.h>
#include <Engine\Constants.h>

using namespace Logic;

#define FADING_TIMER 300.f

iMenu::iMenu(MenuGroup group) : m_group(group), m_drawButtons(false), m_drawMenu(false), m_pressed(true), m_safeToRemove(false), m_isFading(false) { }

iMenu::~iMenu() { }

// Starts the fadein animation, menu can't be removed during this time
void iMenu::fadeIn()
{
    m_fader.startFadeIn(FADING_TIMER);
    m_isFading = true;
    m_safeToRemove = false;
    m_background.alpha = 0.f;
    for (Button& btn : m_buttons)
        btn.setAlpha(0.f);
}

// Starts the fadeout animation, menu can't be removed during this time
void iMenu::fadeOut()
{
    m_fader.startFadeOut(FADING_TIMER);
    m_isFading = true;
    m_safeToRemove = false;
    m_background.alpha = 1.f;
    for (Button& btn : m_buttons)
        btn.setAlpha(1.f);
}

// Adds a texture on as a background, covering the full screen
void iMenu::addBackground(Resources::Textures::Files texture, float alpha)
{
    m_background.texture = texture;
    m_background.alpha = alpha;
    m_background.screenRect.topLeft = DirectX::SimpleMath::Vector2(0, 0);
    m_background.screenRect.bottomRight = DirectX::SimpleMath::Vector2(1.f, 1.f);
    m_background.textureRect.topLeft = DirectX::SimpleMath::Vector2(0, 0);
    m_background.textureRect.bottomRight = DirectX::SimpleMath::Vector2(1.f, 1.f);
    m_drawMenu = true;
}

// Simple builder function of buttons, for internal use only (iMenuFactory.cpp)
void iMenu::addButton(ButtonData btn)
{
    Button temp(btn.screenRect.topLeft.x * WIN_WIDTH, btn.screenRect.topLeft.y * WIN_WIDTH, 
        (btn.screenRect.bottomRight.x - btn.screenRect.topLeft.x) * WIN_WIDTH,
        (btn.screenRect.bottomRight.y - btn.screenRect.topLeft.y) * WIN_WIDTH,
        btn.texture, btn.texRectNormal, btn.texRectHover, btn.texRectActive, btn.callback);

    m_buttons.push_back(temp);
    m_drawButtons = true;
}

// Updates the buttons of this menu
void iMenu::update(int x, int y, float deltaTime)
{
    if (m_isFading)
    {
        m_fader.update(deltaTime);

        float currentAlpha = m_fader.getCurrentProcent();
        m_background.alpha = currentAlpha;
        for (Button& btn : m_buttons)
            btn.setAlpha(currentAlpha);

        if (m_fader.m_complete)
        {
            m_isFading = false;
            if (m_fader.m_style == FadeOut)
                m_safeToRemove = true;
        }
    }
    else
    {
        updateClick(x, y);
        updateHover(x, y);
    }
}

// Updates the buttons on-press states of this menu
void iMenu::updateClick(int x, int y)
{
    bool clickWasPressed = DirectX::Mouse::Get().GetState().leftButton;
    if (clickWasPressed && !m_pressed)
    {
        m_pressed = true;
        for (Button& btn : m_buttons)
            btn.updateOnPress(x, y);
    }
    else if (!clickWasPressed && m_pressed)
    {
        m_pressed = false;
    }
}

// Updates the button's hover states
void iMenu::updateHover(int x, int y)
{
    for (Button& btn : m_buttons)
        btn.hoverOver(x, y);
}

// Render the background at the back and the buttons in front
void iMenu::render() const
{
    if (m_drawMenu)
        QueueRender(m_background);

    if (m_drawButtons)
        for (const Button& btn : m_buttons)
            btn.render();
}