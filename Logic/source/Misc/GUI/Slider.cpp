#include <Misc\GUI\Slider.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Graphics\include\MainCamera.h>
using namespace Logic;

Slider::Slider(
    std::string name,
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
    float delimiter)
{
    m_inactive = FloatRect(inactive);
    m_active = FloatRect(active);
    m_hover = FloatRect(hover);
    m_name = name;
    m_animationStart = DirectX::SimpleMath::Vector2(0, 0);
    m_animationEnd = DirectX::SimpleMath::Vector2(0, 0);
    m_y = y;
    m_width = width;
    m_height = height;
    m_min = min;
    m_max = max;
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_delimiter = 1 / delimiter;
    m_value = value;

    m_value = value;
    m_tempValue = *m_value;

    if (*m_value > maxValue)
    {
        *m_value = maxValue;
    }
    else if (*m_value < minValue)
    {
        *m_value = minValue;
    }
    float X = x;
    X = (((*m_value - m_minValue) / (m_maxValue - m_minValue)) * (m_max - m_min)) + m_min;

    FloatRect screenRect = {
        (X + (m_width * 0.5f)) / WIN_WIDTH,
        y / WIN_HEIGHT,
        width / WIN_WIDTH,
        height / WIN_HEIGHT
    };

    state = INACTIVE;
    renderInfo.texture = texture;
    renderInfo.screenRect = screenRect;
    renderInfo.textureRect = inactive;
    renderInfo.alpha = 1;

    m_textRenderInfo.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfo.font = Resources::Fonts::KG14;
    m_textRenderInfo.position = DirectX::SimpleMath::Vector2(m_max + 20.0f, m_y);

    if (m_name.compare("FOVSlider") == 0)
    {
        int test = *m_value;
        m_textInput = std::to_wstring(test);
    }
    else if (m_name.compare("MouseSlider") == 0)
    {
        int test = (*m_value + 0.000001) * 1000;
        m_textInput = std::to_wstring(test);
    }
    else
    {
        int test = (*m_value + 0.000001) * 100;
        m_textInput = std::to_wstring(test);
    }
    m_textRenderInfo.text = m_textInput.c_str();
}

Slider::~Slider()
{
}

bool Slider::updateOnPress(int posX, int posY)
{
    if (renderInfo.screenRect.contains((float(posX) + m_width) / WIN_WIDTH, float(posY) / WIN_HEIGHT) || this->state == ACTIVE)
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

            m_tempValue = (m_maxValue - m_minValue) * ((posx - m_min) / (m_max - m_min));

            float flooredFinalValue = floorf(m_delimiter * m_tempValue + 0.5f) / m_delimiter;
           
            posx = ((flooredFinalValue / (m_maxValue - m_minValue)) * (m_max - m_min)) + m_min;
           
            m_tempValue = flooredFinalValue;

            m_tempValue += m_minValue;

            FloatRect screenRect = {
                (posx + (m_width * 0.5f)) / WIN_WIDTH,
                 m_y / WIN_HEIGHT,
                 m_width / WIN_WIDTH,
                 m_height / WIN_HEIGHT
            };
             

             
             renderInfo.screenRect = screenRect;
             m_textRenderInfo.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
             m_textRenderInfo.font = Resources::Fonts::KG14;
             m_textRenderInfo.position = DirectX::SimpleMath::Vector2(m_max + 20.0f, m_y);
             if (m_name.compare("FOVSlider") == 0)
             {
                 int test = m_tempValue;
                 m_textInput = std::to_wstring(test);
             }
             else if (m_name.compare("MouseSlider") == 0)
             {
                 int test = (m_tempValue + 0.000001) * 1000;
                 m_textInput = std::to_wstring(test);
             }
             else
             {
                 int test = (m_tempValue + 0.000001) * 100;
                 m_textInput = std::to_wstring(test);
             }
             m_textRenderInfo.text = m_textInput.c_str();

             return true;
        }
    }
    else
    {
        setState(INACTIVE);
        return false;
    }
}

void Logic::Slider::updateOnRelease(int posX, int posY)
{
    if (renderInfo.screenRect.contains((float(posX) + m_width) / WIN_WIDTH, float(posY) / WIN_HEIGHT) || this->state == ACTIVE)
    {
        setState(Slider::INACTIVE);
        *m_value = m_tempValue;

        if (m_name.compare("FOVSlider") == 0)
        {
            Global::mainCamera->updateFOV(*m_value);
            std::cout << "Entered FOV";
        }
        else if (m_name.compare("MasterSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_MASTER, *m_value);
        }
        else if (m_name.compare("MusicSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_MUSIC, *m_value);
        }
        else if (m_name.compare("AmbienceSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_AMBIENT, *m_value);
        }
        else if (m_name.compare("SFXSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_SFX, *m_value);
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
        renderInfo.textureRect = m_inactive;
        break;
    case ACTIVE:
        renderInfo.textureRect = m_active;
        break;
    case HOVER:
        renderInfo.textureRect = m_hover;
        break;
    }
}

void Slider::setAlpha(float alpha)
{
    renderInfo.alpha = alpha;
}

void Slider::setUVS(FloatRect newUVs)
{
    m_inactive = newUVs;
    m_active = newUVs;
    m_hover = newUVs;
}

void Slider::render() const
{
    QueueRender(renderInfo);
    QueueRender(m_textRenderInfo);
}
