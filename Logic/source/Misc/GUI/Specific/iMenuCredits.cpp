#include <Misc\GUI\Specific\iMenuCredits.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Misc\ComboMachine.h>

using namespace Logic;

iMenuCredits::iMenuCredits(iMenu::MenuGroup group)
    : iMenu(group) { 
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::MUSIC_CREDITS, nullptr, true);

    scoreText.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f);
    scoreText.font = Resources::Fonts::KG14;
    scoreText.isMoveable = false;
    scoreText.position = DirectX::SimpleMath::Vector2(100, 240);

    scoreText.text = L"Your score:" + std::to_wstring(ComboMachine::Get().getTotalScore());
}

void iMenuCredits::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);
    credits.update(deltaTime);
}

void iMenuCredits::render() const
{
    iMenu::render();
    credits.render();
    QueueRender(scoreText);
}
