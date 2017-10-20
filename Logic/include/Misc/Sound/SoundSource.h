#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include <Misc\Sound\NoiseStructs.h>

namespace Logic
{
	class SoundSource
	{
	public:
		SoundSource();

		void update();
		void playSFX(SFX sfx);
		void playMusic(MUSIC music);

	public:
		FMOD_VECTOR		pos;
		FMOD_VECTOR		vel;
		FMOD::Channel*	channel;

	};
}

#endif // !SOUNDSOURCE_H
