#include <Misc\GUI\Slider.h>
#include <Misc\Sound\NoiseMachine.h>
using namespace Logic;

Slider::Slider(
    float x, float y, 
    float width, float height, 
    Resources::Textures::Files texture, 
    FloatRect inactive, 
    FloatRect active, 
    FloatRect hover) : inactive(inactive), active(active), hover(active)
{
    m_animationStart = DirectX::SimpleMath::Vector2(0, 0);
    m_animationEnd = DirectX::SimpleMath::Vector2(0, 0);
    m_y = y;
    m_width = width;
    m_height = height;
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

Slider::~Slider()
{
}

void Slider::updateOnPress(int posX, int posY)
{
    if (renderInfo.screenRect.contains(float(posX) / WIN_WIDTH, float(posY) / WIN_HEIGHT))
    {
        if (this->state != ACTIVE)
        {
            Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
            setState(ACTIVE);
        }
        else if (this->state == ACTIVE)
        {
            FloatRect screenRect = {
                float(posX) / WIN_WIDTH,
                m_y / WIN_HEIGHT,
                m_width / WIN_WIDTH,
                m_height / WIN_HEIGHT
            };
            renderInfo.screenRect = screenRect;
        }
        else
        {
            setState(INACTIVE);
        }
    }
}

void Slider::hoverOver(int posX, int posY)
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

bool Slider::animationTransition(float dt, float maxAnimationTime, bool forward)
{
    return false;
}

void Slider::setState(State state)
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

void Slider::setAlpha(float alpha)
{
    renderInfo.alpha = alpha;
}

void Slider::setUVS(FloatRect newUVs)
{
    this->inactive = newUVs;
    this->active = newUVs;
    this->hover = newUVs;
}

void Slider::render() const
{
    QueueRender(renderInfo);
}
