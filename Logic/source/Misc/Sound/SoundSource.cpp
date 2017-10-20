#include <Misc\Sound\SoundSource.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

SoundSource::SoundSource() : channel(nullptr), pos(), vel()
{
	noiseMachine = &NoiseMachine::Get();
	autoPlayer = nullptr;
}

SoundSource::~SoundSource()
{
	if (autoPlayer)
	{
		delete autoPlayer;
		autoPlayer = nullptr;
	}
}

// Updates this entity's 3D position and autoplayer
void SoundSource::update(float deltaTime)
{
	if (channel)
	{
		channel->set3DAttributes(&pos, &vel);
	}

	if (autoPlayer)
	{
		if (autoPlayer->checkIfPlay(deltaTime))
		{
			channel->setFrequency(autoPlayer->pitch.value);
			noiseMachine->playSFX(autoPlayer->sfx, this, false);
		}
	}
}

// Simply plays a SFX one time
void SoundSource::playSFX(SFX sfx)
{
	noiseMachine->playSFX(sfx, this, false);
}

// Simply plays one song one time
void SoundSource::playMusic(MUSIC music)
{
	noiseMachine->playMusic(music, this, false);
}

// Plays a SFX inbetween intervals, example: autoPlaySFX(SFX::BOING, 5000.f, 500.f); - plays the BOING sfx every 5000 ms, with a 500 ms random variation
void SoundSource::autoPlaySFX(SFX sfx, float timeBetween, float timeOffset, float pitch, float pitchOffset)
{
	if (autoPlayer)	delete autoPlayer;
	autoPlayer = new AutoPlayer(sfx, timeBetween, timeOffset, pitch, pitchOffset);
}

/*	******************	*
	AUTO PLAYER STRUCT
*	******************	*/
SoundSource::AutoPlayer::AutoPlayer(SFX inSFX, float inTimeBetween, float inTimeOffset, float inPitch, float inPitchOffset)
{
	sfx = inSFX;
	time = Offset(inTimeBetween, inTimeOffset);
	pitch = Offset(inPitch, inPitchOffset);
}

// Checks if the timer have run out
bool SoundSource::AutoPlayer::checkIfPlay(float deltaTime)
{
	time.value -= deltaTime;
	if (time.value < NULL)
	{
		reset();
		return true;
	}
	return false;
}

// Reset new variables with random generator
void SoundSource::AutoPlayer::reset()
{
	time.value = time.original + RandomGenerator::singleton().getRandomFloat(-time.offset, time.offset);
	pitch.value = pitch.original + RandomGenerator::singleton().getRandomFloat(-pitch.offset, pitch.offset);
}

/*	*************	*
	OFFSET STRUCT
*	*************	*/
SoundSource::Offset::Offset() : value(1.f), original(1.f), offset(0.f) { }
SoundSource::Offset::Offset(float inValue, float inOffset) : value(inValue), original(inValue), offset(inOffset) { }