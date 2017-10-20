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

		void autoPlaySFX(SFX sfx, float timeBetween, float timeOffset, float pitch = 1, float pitchOffset = 0);

	public:
		AutoPlayer*		autoPlayer;
		FMOD_VECTOR		pos;
		FMOD_VECTOR		vel;
		FMOD::Channel*	channel;
		NoiseMachine*	noiseMachine;
	};
}

#endif // !SOUNDSOURCE_H
