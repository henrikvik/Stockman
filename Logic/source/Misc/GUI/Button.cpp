#include <Misc\GUI\Button.h>
using namespace Logic;


Logic::Button::Button(
    float x, float y,
    float width, float height,
    Resources::Textures::Files texture,
    FloatRect inactive,
    std::function<void(void)> callback
)
    : Button(x, y, width, height, texture, inactive, inactive, callback)
{
}

Logic::Button::Button(
    float x, float y,
    float width, float height,
    Resources::Textures::Files texture,
    FloatRect inactive,
    FloatRect active,
    std::function<void(void)> callback
)
    : Button(x, y, width, height, texture, inactive, active, active, callback)
{
}

Logic::Button::Button(
    float x, float y,
    float width, float height,
    Resources::Textures::Files texture,
    FloatRect inactive, 
    FloatRect active, 
    FloatRect hover,
    std::function<void(void)> callback
)
    : inactive(inactive)
    , active(hover)
    , hover(active)
    , callback(callback)
{
    m_animationStart = DirectX::SimpleMath::Vector2(0,0);
    m_animationEnd   = DirectX::SimpleMath::Vector2(0,0);

    FloatRect screenRect = {
        x / WIN_WIDTH,
        y / WIN_HEIGHT,
        width / WIN_WIDTH,
        height / WIN_HEIGHT
    };

    state = INACTIVE;
    renderInfo.texture = texture;
    renderInfo.screenRect = screenRect;
    renderInfo.textureRect = inactive;
    renderInfo.alpha = 1;
}

Button::~Button()
{
}

void Logic::Button::setCallback(std::function<void(void)> callback)
{
    this->callback = callback;
}

void Button::updateOnPress(int posX, int posY)
{
	if (callback && renderInfo.screenRect.contains(float(posX) / WIN_WIDTH, float(posY) / WIN_HEIGHT))
	{
        if (this->state != ACTIVE)
        {
            setState(ACTIVE);
        }
        else
        {
            setState(INACTIVE);
        }
        
        callback();
	}
}

void Button::hoverOver(int posX, int posY)
{
    if (this->state != ACTIVE)
    {
        if (renderInfo.screenRect.contains(float(posX) / WIN_WIDTH, float(posY) / WIN_HEIGHT))
        {
            setState(HOVER);
        }
        else
        {
            setState(INACTIVE);
        }
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
	
    //screenRect.x = lerpResult.x;
    //screenRect.y = lerpResult.y;

	return done;
}

void Logic::Button::setState(State state)
{
    this->state = state;
    switch (state)
    {
    case INACTIVE:
        renderInfo.textureRect = inactive;
        break;
    case ACTIVE:
        renderInfo.textureRect = active;
        break;
    case HOVER:
        renderInfo.textureRect = hover;
        break;
    }
}

void Logic::Button::render() const
{
    QueueRender(renderInfo);
}
