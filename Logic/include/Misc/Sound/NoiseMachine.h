#ifndef NOISEMACHINE_H
#define NOISEMACHINE_H

#include <Engine\Constants.h>	// Paths to folders
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <stdarg.h>
#include <string>
#include <btBulletCollisionCommon.h>

#define SOUND_SFX_PATH "Resources/Sound/SFX/"
#define SOUND_MUSIC_PATH "Resources/Sound/Music/"

/* 

	// if it eats up memory
	ERRCHECK(FMOD::Memory_Initialize(malloc(4*1024*1024), 4*1024*1024, 0,0,0));

*/

namespace Logic
{
	static const int MAX_CHANNELS	= 32;
	static const int MAX_SFX		= 32;
	static const int MAX_SONGS		= 32;

	struct SoundData
	{
		SoundData() : pos({0, 0, 0}), radius(100.f) { }
		SoundData(btVector3 inPos, btScalar inRadius) : pos(inPos), radius(inRadius) { }

		btVector3 pos;
		btScalar radius;
	};

	enum SFX { TEST };
	enum MUSIC { BOOM };

	class NoiseMachine
	{
	public:
		static NoiseMachine& Get()
		{
			static NoiseMachine noiceMachine;
			return noiceMachine;
		}

		void init();
		void update();

		void playSFX(SoundData sData, SFX sfx, ...);
		void playMusic(SoundData sData, MUSIC music, ...);

	private:

		// Initializing
		void initSFX();
		void initMusic();
		FMOD_RESULT createSound(SFX sfx, std::string path);
		FMOD_RESULT createSound(MUSIC music, std::string path);

		// Debugging and Errors
		void CRASH_EVERYTHING(const char *format, ...);
		void ERRCHECK(FMOD_RESULT result);

		FMOD::System*	m_system;
		FMOD::Channel*	m_channel;
		FMOD::Sound*	m_sound[MAX_SFX];
		FMOD::Sound*	m_music[MAX_SFX];
	};
}

#endif // !NOICEMACHINE_H
