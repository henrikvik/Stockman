#include <Misc\GUI\Slider.h>
#include <Misc\Sound\NoiseMachine.h>
using namespace Logic;

Slider::Slider(
    float x, float y, 
    float width, float height, 
    Resources::Textures::Files texture, 
    FloatRect inactive, 
    FloatRect active, 
    FloatRect hover,
    float min,
    float max,
    float* value,
    float minValue,
    float maxValue,
    float delimiter) : inactive(inactive), active(active), hover(active)
{
    m_animationStart = DirectX::SimpleMath::Vector2(0, 0);
    m_animationEnd = DirectX::SimpleMath::Vector2(0, 0);
    m_y = y;
    m_width = width;
    m_height = height;
    m_min = min;
    m_max = max;
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_delimiter = delimiter;

    m_value = value;
    float X = x;
    X = (((*m_value - m_minValue) / (m_maxValue - m_minValue)) * (m_max - m_min)) + m_min - (m_width * 0.5f);
   /* *m_value = m_minValue + (m_maxValue - m_minValue) * ((x - m_min) / (m_max - m_min));*/
    FloatRect screenRect = {
        X  / WIN_WIDTH,
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
    if (renderInfo.screenRect.contains(float(posX) / WIN_WIDTH, float(posY) / WIN_HEIGHT) || this->state == ACTIVE)
    {
        if (this->state != ACTIVE)
        {
            Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
            setState(ACTIVE);
        }
        else if (this->state == ACTIVE)
        {

            float posx = float(posX);

            if (posx < m_min)
            {
                posx = m_min;
            }
            else if (posx > m_max)
            {
                posx = m_max;
            }

            *m_value = m_minValue + (m_maxValue - m_minValue) * ((posx - m_min) / (m_max - m_min));
           /* float flooredFinalValue = floorf(finalValue);

            if (finalValue - flooredFinalValue >= 0.5f)
            {
                flooredFinalValue++;
            }

            posx = ((flooredFinalValue / (m_maxValue - m_minValue)) * (m_max - m_min)) + m_min;*/

             FloatRect screenRect = {
                 (posx - (m_width * 0.5f)) / WIN_WIDTH,
                 m_y / WIN_HEIGHT,
                 m_width / WIN_WIDTH,
                 m_height / WIN_HEIGHT
            };
             

             
             renderInfo.screenRect = screenRect;
        }
    }
    else
    {
        setState(INACTIVE);
    }
}

void Logic::Slider::updateOnRelease(int posX, int posY)
{
    if (renderInfo.screenRect.contains(float(posX) / WIN_WIDTH, float(posY) / WIN_HEIGHT) || this->state == ACTIVE)
    {
        setState(Slider::INACTIVE);
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
