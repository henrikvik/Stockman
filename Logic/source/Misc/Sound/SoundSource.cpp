#include <Misc\Sound\SoundSource.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

SoundSource::SoundSource() : channel(nullptr), pos(), vel(), autoPlayer(nullptr), delayPlayer(nullptr)
{
	noiseMachine = &NoiseMachine::Get();
}

SoundSource::~SoundSource()
{
	if (autoPlayer)
	{
		delete autoPlayer;
		autoPlayer = nullptr;
	}
	if (delayPlayer)
	{
		delete delayPlayer;
		delayPlayer = nullptr;
	}
}

// Updates this entity's 3D position and autoplayer
void SoundSource::update(float deltaTime)
{
	if (autoPlayer)
	{
		if (autoPlayer->checkIfPlay(deltaTime))
		{
			noiseMachine->playSFX(autoPlayer->sfx, this, false);
			channel->set3DAttributes(&pos, &vel);
		}
	}

	if (delayPlayer)
	{
		if (delayPlayer->checkIfDone(deltaTime))
		{
			(delayPlayer->id) ? noiseMachine->playMusic(MUSIC(delayPlayer->type), this, true) :
								noiseMachine->playSFX(SFX(delayPlayer->type), this, true);
			delete delayPlayer;
			delayPlayer = nullptr;
		}
	}

	if (channel)
	{
		channel->set3DAttributes(&pos, &vel);
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

// Plays a SFX but after a specific delay of time in ms
void SoundSource::delayPlaySFX(SFX sfx, float delay)
{
	if (delayPlayer) delete delayPlayer;
	delayPlayer = new DelayPlayer(0, sfx, delay);
}

// Plays a song but after a specific delay of time in ms
void SoundSource::delayPlayMusic(MUSIC music, float delay)
{
	if (delayPlayer) delete delayPlayer;
	delayPlayer = new DelayPlayer(1, music, delay);
}

// Plays a SFX inbetween intervals, example: autoPlaySFX(SFX::BOING, 5000.f, 500.f); - plays the BOING sfx every 5000 ms, with a 500 ms random variation
void SoundSource::autoPlaySFX(SFX sfx, float timeBetween, float timeOffset, float pitch, float pitchOffset)
{
	if (autoPlayer)	delete autoPlayer;
	autoPlayer = new AutoPlayer(sfx, timeBetween, timeOffset, pitch, pitchOffset);
}

FMOD_VECTOR* SoundSource::getPosition()
{
	return &pos;
}

FMOD_VECTOR* SoundSource::getVelocity()
{
	return &vel;
}

// Use with caution, can be nullptr if no active sound
FMOD::Channel * SoundSource::getChannel()
{
	return channel;
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

/*	*******************		*
	DELAY PLAYER STRUCT 
*	*******************		*/
SoundSource::DelayPlayer::DelayPlayer(__int8 inID, __int8 inType, float inTimer) : id(inID), type(inType), timer(inTimer) { }

bool SoundSource::DelayPlayer::checkIfDone(float deltaTime)
{
	timer -= deltaTime;
	return (timer < NULL);
}

/*	*************	*
	OFFSET STRUCT
*	*************	*/
SoundSource::Offset::Offset() : value(1.f), original(1.f), offset(0.f) { }
SoundSource::Offset::Offset(float inValue, float inOffset) : value(inValue), original(inValue), offset(inOffset) { }