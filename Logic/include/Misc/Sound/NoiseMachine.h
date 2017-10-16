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

namespace Logic
{
	static const int MAX_CHANNELS	= 1000;
	static const int MAX_GROUPS		= 3;
	static const int MAX_SFX		= 32;
	static const int MAX_SONGS		= 32;

	enum CHANNEL_GROUP {
		CHANNEL_SFX,
		CHANNEL_MUSIC,
		CHANNEL_AMBIENT
	};

	enum SFX {
		TEST
	};

	enum MUSIC {
		BOOM
	};

	struct SoundSettings
	{
		SoundSettings() 
		{ 
			volume[CHANNEL_SFX]			= 1.f;
			volume[CHANNEL_MUSIC]		= 1.f;
			volume[CHANNEL_AMBIENT]		= 1.f;
			pitch[CHANNEL_SFX]			= 1.f;
			pitch[CHANNEL_MUSIC]		= 1.f;
			pitch[CHANNEL_AMBIENT]		= 1.f;
		}

		float pitch[MAX_CHANNELS];	//< Between 0:1
		float volume[MAX_CHANNELS];	//< Between 0:1
	};

	struct Sound
	{
		Sound() : channel(nullptr), data(nullptr), group(CHANNEL_GROUP(0)) { }

		void setVolume(float vol)								
		{ channel->setVolume(vol); }
		void setPitch(float pitch)								
		{ channel->setPitch(pitch); }
		void set3DAttributes(FMOD_VECTOR pos, FMOD_VECTOR vel)	
		{ channel->set3DAttributes(&pos, &vel); }

		FMOD::Channel*	channel;
		FMOD::Sound*	data;
		CHANNEL_GROUP	group;
	};

	struct ListenerData
	{
		ListenerData() : id(0), vel({ 0.f, 1.f, 0.f }), up({ 0.f, 1.f, 0.f }), forward({ 0.f, 0.f, 1.f }), pos({0.f, 0.f, 0.f}) { }
		void update(btVector3 inVelocity, btVector3 inUp, btVector3 inForward, btVector3 inPosition)
		{
			vel			= { inVelocity.x(), inVelocity.y(), inVelocity.z()	};
			up			= { inUp.x(),		inUp.y(),		inUp.z()		};
			forward		= { inForward.x(),	inForward.y(),	inForward.z()	};
			pos			= { inPosition.x(), inPosition.y(), inPosition.z()	};
		}

		__int8		id;			//< Different id for different players
		FMOD_VECTOR vel;
		FMOD_VECTOR up;
		FMOD_VECTOR forward;
		FMOD_VECTOR pos;
	};

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
		Sound*			m_sfx[MAX_SFX];
		Sound*			m_music[MAX_SONGS];
	};
}

#endif // !NOICEMACHINE_H
