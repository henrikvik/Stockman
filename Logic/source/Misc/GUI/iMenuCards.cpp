#include <Misc\GUI\iMenuCards.h>
#include <Misc\Card.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

#define MAX_CARDS 3

iMenuCards::iMenuCards(iMenu::MenuGroup group)
    : iMenu(group) 
{ 
    m_renderable = false;
   
    // Setup of the "round complete" sprite
    m_spriteRenderInfo.alpha = 0.f;
    m_spriteRenderInfo.isMoveable = true;
    m_spriteRenderInfo.texture = Resources::Textures::WaveComplete;
    m_spriteRenderInfo.screenRect = FloatRect(
    { 0.0f,
        0.0f },
        { 1.f,
        1.f }
    );
    m_spriteRenderInfo.textureRect = FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f });

    Sound::NoiseMachine::Get().playSFX(Sound::SFX::UPGRADE_UNLOCKED, nullptr, true);
    Sound::NoiseMachine::Get().playSFX(Sound::SFX::WAVE_END, nullptr, true);
}

iMenuCards::~iMenuCards() { }

void iMenuCards::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    m_spriteRenderInfo.alpha = m_fader.getCurrentPercentage();
}


void iMenuCards::render() const
{
    QueueRender(m_spriteRenderInfo);
}


