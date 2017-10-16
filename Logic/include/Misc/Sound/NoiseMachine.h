#ifndef NOISEMACHINE_H
#define NOISEMACHINE_H

#include <Engine\Constants.h>	// Paths to folders
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <stdarg.h>
#include <string>
#include <btBulletCollisionCommon.h>
#include <Misc\Sound\NoiseStructs.h>

#define SOUND_SFX_PATH "Resources/Sound/SFX/"
#define SOUND_MUSIC_PATH "Resources/Sound/Music/"

namespace Logic
{
	class NoiseMachine
	{
	public:
		static NoiseMachine& Get()
		{
			static NoiseMachine noiceMachine;
			return noiceMachine;
		}

		void init(SoundSettings settings);
		void clear();
		void update(ListenerData& listener);

		void playSFX(SFX sfx, bool overdrive = false);
		void playMusic(MUSIC music, bool overdrive = false);

	private:

		// Initializing
		void initSFX();
		void initMusic();

		void play(Sound* sound, bool overdrive);

		FMOD_RESULT createSound(SFX sfx, CHANNEL_GROUP group, std::string path, FMOD_MODE mode);
		FMOD_RESULT createSound(MUSIC music, CHANNEL_GROUP group, std::string path, FMOD_MODE mode);

		// Debugging and Errors
		void DEBUG_SOUND(Sound* sound);
		void CRASH_EVERYTHING(const char *format, ...);
		void ERRCHECK(FMOD_RESULT result);

		SoundSettings	m_settings;
		FMOD::System*	m_system;
		Sound*			m_sfx[THRESHOLD::MAX_SFX];
		Sound*			m_music[THRESHOLD::MAX_SONGS];
	};
}

#endif // !NOICEMACHINE_H
