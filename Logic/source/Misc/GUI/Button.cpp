#include <Misc\GUI\Button.h>
using namespace Logic;


Logic::Button::Button(
    Resources::Textures::Files texture,
    DirectX::SimpleMath::Rectangle screenRect,
    DirectX::SimpleMath::Rectangle inactive,
    std::function<void(void)> callback
)
    : Button(texture, screenRect, inactive, inactive, callback)
{
}

Logic::Button::Button(
    Resources::Textures::Files texture, 
    DirectX::SimpleMath::Rectangle screenRect, 
    DirectX::SimpleMath::Rectangle inactive,
    DirectX::SimpleMath::Rectangle active,
    std::function<void(void)> callback
)
    : Button(texture, screenRect, inactive, active, active, callback)
{
}

Logic::Button::Button(
    Resources::Textures::Files texture, 
    DirectX::SimpleMath::Rectangle screenRect, 
    DirectX::SimpleMath::Rectangle inactive, 
    DirectX::SimpleMath::Rectangle active, 
    DirectX::SimpleMath::Rectangle hover,
    std::function<void(void)> callback
)
    : texture(texture)
    , screenRect(screenRect)
    , inactive(inactive)
    , active(active)
    , hover(hover)
    , callback(callback)
{
    textureRect = &inactive;
}

Button::~Button()
{
}

void Logic::Button::setCallback(std::function<void(void)> callback)
{
    callback = callback;
}

void Button::updateOnPress(int posX, int posY)
{
	if (callback && m_buttonInfo.m_rek.Contains(posX, posY))
	{
        callback();
	}
}

void Button::hoverOver(int posX, int posY)
{
    if (m_buttonInfo.m_rek.Contains(posX, posY))
    {
        state = HOVER;
    }
    else
    {
        state = INACTIVE;
    }
}

bool Button::animationTransition(float dt, float maxAnimationTime, bool forward)
{
	m_animationTime += dt * (1 / maxAnimationTime);
	bool done = false;

	if (m_animationTime >= 1)
	{
		m_animationTime = 0;
		done = true;
	}
 
	DirectX::SimpleMath::Vector2 lerpResult;
	if (forward)
		lerpResult = DirectX::SimpleMath::Vector2::Lerp(m_animationStart, m_animationEnd, done ? 1 : m_animationTime);
	else
		lerpResult = DirectX::SimpleMath::Vector2::Lerp(m_animationEnd, m_animationStart, done ? 1 : m_animationTime);
	
    screenRect.x = lerpResult.x;
    screenRect.y = lerpResult.y;

	return done;
}

Graphics::ButtonInfo* Button::getButtonInfo()
{
	return &m_buttonInfo;
}

void Button::setStartAndEnd(float start, float end)
{
    m_start = start;
    m_end = end;
}

void Logic::Button::setState(State state)
{
    this->state = state;
    switch (state)
    {
    case INACTIVE:
        textureRect = &inactive;
        break;
    case ACTIVE:
        textureRect = &active;
        break;
    case HOVER:
        textureRect = &hover;
        break;
    }
}

void Logic::Button::render() const
{
    SpriteRenderInfo renderInfo = {};
    renderInfo.texture = texture;
    renderInfo.screenRect = screenRect;
    renderInfo.textureRect = *textureRect;

    RenderQueue::get().queue(&renderInfo);
}
