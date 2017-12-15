#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderQueue.h>
#include <Engine\Constants.h>
#include <Misc\GUI\iMenuFX.h>

using namespace Logic;

#define DEFAULT_FADING_TIMER     300.f

// Toggle
#define USE_UNIQUE_FADING_TIMERS true  

// Gives us the ability to give each menu a different fading time
#if USE_UNIQUE_FADING_TIMERS

// Give menus their unique timer here in milliseconds
#define FADING_TIMER_INTRO      250.f      
#define FADING_TIMER_GAMEOVER   1000.f
#define FADING_TIMER_SKILL      800.f
#define FADING_TIMER_CARD       500.f

#endif
float getFadingTimer(iMenu::MenuGroup group)
{
#if USE_UNIQUE_FADING_TIMERS
    switch (group)
    {
    case iMenu::MenuGroup::Intro:       return FADING_TIMER_INTRO;
    case iMenu::MenuGroup::GameOver:    return FADING_TIMER_GAMEOVER;
    case iMenu::MenuGroup::Skill:       return FADING_TIMER_SKILL;
    case iMenu::MenuGroup::CardSelect:  return FADING_TIMER_CARD;
    default:                            return DEFAULT_FADING_TIMER;
    }
#endif
    return DEFAULT_FADING_TIMER;
}

iMenu::iMenu(MenuGroup group) : m_group(group), m_effect(nullptr), m_drawButtons(false), m_drawSliders(false), m_drawMenu(false), m_pressed(true), m_safeToRemove(false), m_isFading(false), m_fadingTimer(getFadingTimer(group)), m_mouseMode(DirectX::Mouse::MODE_ABSOLUTE) 
{ 
    m_effect = nullptr;
    m_sld = nullptr;
}

iMenu::~iMenu() 
{
    if (m_effect) 
        delete m_effect; 
}

void iMenu::removeButtons()
{
    m_buttons.clear();
}

void iMenu::removeSliders()
{
    m_sliders.clear();
}

// Starts the fadeIn animation, menu's can't be changed/removed during this time
void iMenu::fadeIn()
{
    m_fader.startFadeIn(m_fadingTimer);
    m_isFading      = true;
    m_safeToRemove  = false;
    setAlpha(0.f);
}

// Starts the fadeOut animation, menu's can't be changed/removed during this time
void iMenu::fadeOut()
{
    m_fader.startFadeOut(m_fadingTimer);
    m_isFading      = true;
    m_safeToRemove  = false;
    setAlpha(1.f);
}

// Adds a special effect to this menu
void iMenu::addEffect(iMenuFX* effect)
{
    if (m_effect)
    {
        delete m_effect;
        m_effect = nullptr;
    }

    m_effect = effect;
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

void iMenu::addSlider(SliderData sld)
{
    Slider temp(sld.name, sld.screenRect.topLeft.x * WIN_WIDTH, sld.screenRect.topLeft.y * WIN_WIDTH,
        (sld.screenRect.bottomRight.x - sld.screenRect.topLeft.x) * WIN_WIDTH,
        (sld.screenRect.bottomRight.y - sld.screenRect.topLeft.y) * WIN_WIDTH,
        sld.texture, sld.texRectNormal, sld.texRectHover, sld.texRectActive, sld.min, sld.max, sld.value, sld.minValue, sld.maxValue, sld.delimeter);

    m_sliders.push_back(temp);
    m_drawSliders = true;
}

// Updates the buttons of this menu
void iMenu::update(int x, int y, float deltaTime)
{
    DirectX::Mouse::Get().SetMode(m_mouseMode);

    if (m_effect)
        m_effect->update(deltaTime);

    if (m_isFading)
    {
        m_fader.update(deltaTime);
        setAlpha(m_fader.getCurrentPercentage());

        if (m_fader.complete)
        {
            m_isFading = false;
            if (m_fader.style == Fader::FadeOut)
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
    if (clickWasPressed)
    {
        if (!m_pressed)
        {
            if (m_effect)
                m_effect->press(float(x), float(y));

            m_pressed = true;
            for (Button& btn : m_buttons)
                btn.updateOnPress(x, y);
        }

        if (m_sld == nullptr)
        {
            for (Slider& sld : m_sliders)
            {
                if (sld.updateOnPress(x, y))
                {
                    m_sld = &sld;
                    break;
                }
            }
        }
        else
        {
            m_sld->updateOnPress(x, y);
        }
    }
    else if (!clickWasPressed && m_pressed)
    {

        if (m_sld != nullptr)
        {
            m_sld->updateOnRelease(x, y);
            m_sld = nullptr;
        }
        m_pressed = false;
    }
}

// Updates the button's hover states
void iMenu::updateHover(int x, int y)
{
    for (Button& btn : m_buttons)
        btn.hoverOver(x, y);

    for (Slider& sld : m_sliders)
        sld.hoverOver(x, y);
}

// Render the background at the back and the buttons in front
void iMenu::render() const
{
    if (m_drawMenu)
        QueueRender(m_background);

    if (m_drawButtons)
        for (const Button& btn : m_buttons)
            btn.render();

    if (m_drawSliders)
        for (const Slider& sld : m_sliders)
            sld.render();

    if (m_effect)
        m_effect->render();
}

// Sets alpha on both menu's and buttons
void iMenu::setAlpha(float alpha)
{
    m_background.alpha = alpha;
    for (Button& btn : m_buttons)
        btn.setAlpha(alpha);

    for (Slider& sld : m_sliders)
        sld.setAlpha(alpha);
}