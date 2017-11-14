#include <Misc\GUI\iMenu.h>
#include <Graphics\include\RenderQueue.h>
#include <Engine\Constants.h>

using namespace Logic;

iMenu::iMenu(MenuGroup group) : m_group(group) { }

iMenu::~iMenu() { }

void iMenu::addBackground(Resources::Textures::Files texture, float alpha)
{
    m_background.texture = texture;
    m_background.alpha = alpha;
    m_background.screenRect.topLeft = DirectX::SimpleMath::Vector2(0, 0);
    m_background.screenRect.bottomRight = DirectX::SimpleMath::Vector2(1.f, 1.f);
    m_background.textureRect.topLeft = DirectX::SimpleMath::Vector2(0, 0);
    m_background.textureRect.bottomRight = DirectX::SimpleMath::Vector2(1.f, 1.f);
}

void iMenu::addButton(ButtonData btn)
{
    Button temp(btn.screenRect.topLeft.x * WIN_WIDTH, btn.screenRect.topLeft.y * WIN_WIDTH,
        (btn.screenRect.bottomRight.x - btn.screenRect.topLeft.x) * WIN_WIDTH,
        (btn.screenRect.bottomRight.y - btn.screenRect.topLeft.y) * WIN_WIDTH,
        btn.texture, btn.texRectNormal, btn.texRectHover, btn.texRectActive, btn.callback);

    m_buttons.push_back(temp);
}

void iMenu::update(int x, int y)
{
    updateClick(x, y);
    updateHover(x, y);
}

void iMenu::updateClick(int x, int y)
{
    if (DirectX::Mouse::Get().GetState().leftButton)
        for (Button& btn : m_buttons)
            btn.updateOnPress(x, y);
}

void iMenu::updateHover(int x, int y)
{
    for (Button& btn : m_buttons)
        btn.hoverOver(x, y);
}

void iMenu::render() const
{
    RenderQueue::get().queue(&m_background);

    for (const Button& btn : m_buttons)
        btn.render();
}