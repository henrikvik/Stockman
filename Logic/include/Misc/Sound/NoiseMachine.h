#ifndef NOISEMACHINE_H
#define NOISEMACHINE_H

#include <Engine\Constants.h>	// Paths to folders
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

		void init();
		void clear();
		void update(ListenerData& listener);

		void playSFX(SFX sfx, bool overdrive = false);
		void playMusic(MUSIC music, bool overdrive = false);

		void setGroupVolume(CHANNEL_GROUP group, float inVolume);
		void setGroupPitch(CHANNEL_GROUP group, float inPitch);
		float getGroupVolume(CHANNEL_GROUP group);
		float getGroupPitch(CHANNEL_GROUP group);

	private:
		void initGroups();
		void initSFX();
		void initMusic();

		void checkIfPlay(Sound* sound, FMOD::Channel* channel, bool overdrive);
		void play(Sound* sound, FMOD::Channel* channel);

		FMOD_RESULT createSound(SFX sfx, CHANNEL_GROUP group, std::string path, FMOD_MODE mode);
		FMOD_RESULT createSound(MUSIC music, CHANNEL_GROUP group, std::string path, FMOD_MODE mode);

		FMOD::System*		m_system;
		FMOD::ChannelGroup*	m_group[THRESHOLD::MAX_GROUPS];
		Sound*				m_sfx[THRESHOLD::MAX_SFX];
		Sound*				m_music[THRESHOLD::MAX_SONGS];

		// Debugging and Errors
		void DEBUG_SOUND(Sound* sound);
		void CRASH_EVERYTHING(const char *format, ...);
		void ERRCHECK(FMOD_RESULT result);
	};
}

#endif // !NOICEMACHINE_H
