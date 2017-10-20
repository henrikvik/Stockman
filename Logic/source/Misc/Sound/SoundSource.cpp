#include <Misc\Sound\SoundSource.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

SoundSource::SoundSource() : channel(nullptr), pos(), vel()
{
//	nm = NoiseMachine::Get();
}

void SoundSource::update()
{
	if (channel)
	{
		channel->set3DAttributes(&pos, &vel);
	}
}

void SoundSource::playSFX(SFX sfx)
{
//	nm.playSFX(sfx, this, false);
}

void SoundSource::playMusic(MUSIC music)
{
//	nm.playMusic(music, this, false);
}