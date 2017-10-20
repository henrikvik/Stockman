#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include <Misc\Sound\NoiseStructs.h>
#include <Misc\RandomGenerator.h>

namespace Logic
{
	class NoiseMachine;
	class SoundSource
	{
	public:

		struct Offset
		{
			Offset();
			Offset(float inValue, float inOffset);
			float value, original, offset;
		};

		struct DelayPlayer
		{
			DelayPlayer(__int8 inID, __int8 inType, float inTimer);
			bool checkIfDone(float deltaTime);

			__int8 id;
			__int8 type;
			float timer;
		};

		struct AutoPlayer
		{
			AutoPlayer(SFX inSFX, float inTimeBetween, float inTimeOffset, float inPitch, float inPitchOffset);
			bool checkIfPlay(float deltaTime);
			void reset();

			SFX sfx;
			Offset time, pitch;
		};

		SoundSource();
		~SoundSource();
		void update(float deltaTime);

		void playSFX(SFX sfx);
		void playMusic(MUSIC music);
		void delayPlaySFX(SFX sfx, float delay);
		void delayPlayMusic(MUSIC music, float delay);
		void autoPlaySFX(SFX sfx, float timeBetween, float timeOffset, float pitch = 1, float pitchOffset = 0);

		FMOD_VECTOR* getPosition();
		FMOD_VECTOR* getVelocity();
		FMOD::Channel* getChannel();

	public:
		DelayPlayer*	delayPlayer;
		AutoPlayer*		autoPlayer;
		FMOD_VECTOR		pos;
		FMOD_VECTOR		vel;
		FMOD::Channel*	channel;
		NoiseMachine*	noiseMachine;
	};
}

#endif // !SOUNDSOURCE_H
