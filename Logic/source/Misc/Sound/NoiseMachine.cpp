#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

// Initializes all sound system & sounds into memory
void NoiseMachine::init()
{
	// Creates a empty system
	FMOD_RESULT result;
	ERRCHECK(FMOD::System_Create(&m_system));

	// Get version & gets the hell out if not correct
	unsigned int version;
	result = m_system->getVersion(&version);
	ERRCHECK(result);
	if (version < FMOD_VERSION)
		CRASH_EVERYTHING("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);

	// Initialize the FMOD system
	void* extraDriverData = nullptr;
	ERRCHECK(m_system->init(THRESHOLD::MAX_CHANNELS, FMOD_INIT_CHANNEL_DISTANCEFILTER, extraDriverData));
	ERRCHECK(result);

	// Specific settings of the system
	m_system->set3DSettings(1.0f, 1.0f, 1.0f);

	// Initialize sounds
	initGroups();
	initSFX();
	initMusic();
}

// Clears sound system & sounds from memory
void NoiseMachine::clear()
{
	// Release sounds
	for (int i = 0; i < THRESHOLD::MAX_SFX; i++)
	{
		if (m_sfx[i] != nullptr)
		{
			ERRCHECK(m_sfx[i]->data->release());
			delete m_sfx[i];
			m_sfx[i] = nullptr;
		}
	}

	// Release music
	for (int i = 0; i < THRESHOLD::MAX_SONGS; i++)
	{
		if (m_music[i] != nullptr)
		{
			ERRCHECK(m_music[i]->data->release());
			delete m_music[i];
			m_music[i] = nullptr;
		}
	}

	// Release system
	ERRCHECK(m_system->close());
	ERRCHECK(m_system->release());
	m_system = nullptr;
}

// Prints out information about the current playing channels (Just for debugging, can be removed, will work without it)
void NoiseMachine::update(ListenerData& listener)
{
	// Sets the new listener "ears"
	ERRCHECK(m_system->set3DListenerAttributes(listener.id, &listener.pos, &listener.vel, &listener.forward, &listener.up));

	// Update the 3D sound engine
	ERRCHECK(m_system->update());

	/*  Remove this */
//	DEBUG_SOUND(m_sfx[SFX::TEST]);
}

void NoiseMachine::playMusic(SFX sfx, SoundSource* soundSource, bool overdrive)
{
	checkIfPlay(m_sfx[sfx], soundSource, overdrive);
}

void NoiseMachine::playMusic(MUSIC music, SoundSource* soundSource, bool overdrive)
{
	checkIfPlay(m_music[music], soundSource, overdrive);
}

// Checks if sound if currently playing and if overdrive should be used
void NoiseMachine::checkIfPlay(Sound* sound, SoundSource* soundSource, bool overdrive)
{
	if (overdrive)
		play(sound, soundSource);
	else
	{
		// Checking if channel is currently playing something
		bool playing = false;
		if (soundSource->channel)
		{
			FMOD_RESULT result = soundSource->channel->isPlaying(&playing);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}
		}

		// If not playing sound, play sound
		if (!playing)
			play(sound, soundSource);
	}
}

// Actually plays the sound
void NoiseMachine::play(Sound* sound, SoundSource* soundSource)
{
	// Playing sound
	ERRCHECK(m_system->playSound(sound->data, m_group[sound->group], false, &soundSource->channel));

	// Saving the created channel onto the sound class for modifications on it
	sound->channel = soundSource->channel;
}

// Initialize all sound groups
void NoiseMachine::initGroups()
{
	// Setting speicific groups
	ERRCHECK(m_system->createChannelGroup("SFX",		&m_group[CHANNEL_GROUP::CHANNEL_SFX]));
	ERRCHECK(m_system->createChannelGroup("MUSIC",		&m_group[CHANNEL_GROUP::CHANNEL_MUSIC]));
	ERRCHECK(m_system->createChannelGroup("AMBIENT",	&m_group[CHANNEL_GROUP::CHANNEL_AMBIENT]));

	// Setting master group
	ERRCHECK(m_system->getMasterChannelGroup(&m_group[CHANNEL_GROUP::CHANNEL_MASTER]));
}

// Initialize all sound effects
void NoiseMachine::initSFX()
{
	// Setting full array as nullptr to avoid crashing
	for (int i = 0; i < THRESHOLD::MAX_SFX; i++) m_sfx[i] = nullptr;

	// Init all the sfx here
	ERRCHECK(createSound(SFX::BOING, CHANNEL_GROUP::CHANNEL_SFX, "test.ogg", FMOD_3D_LINEARROLLOFF));

	// Setting the thresholds of where the listener can hear the sfx
	for (int i = 0; i < THRESHOLD::MAX_SFX; i++)
		if (m_sfx[i])
			m_sfx[i]->data->set3DMinMaxDistance(THRESHOLD::SFX_MIN_DIST, THRESHOLD::SFX_MAX_DIST);
}

// Initialize all music pieces
void NoiseMachine::initMusic()
{
	// Setting full array as nullptr to avoid crashing
	for (int i = 0; i < THRESHOLD::MAX_SONGS; i++) m_music[i] = nullptr;

	// Init all the music here
	ERRCHECK(createSound(MUSIC::NES, CHANNEL_GROUP::CHANNEL_MUSIC, "music.ogg", FMOD_3D_LINEARROLLOFF));

	// Setting the thresholds of where the listener can hear the music
	for (int i = 0; i < THRESHOLD::MAX_SONGS; i++) 
		if (m_sfx[i])
			m_music[i]->data->set3DMinMaxDistance(THRESHOLD::MUSIC_MIN_DIST, THRESHOLD::MUSIC_MAX_DIST);
}

// Allocates a specific sound effect into memory
FMOD_RESULT NoiseMachine::createSound(SFX sfx, CHANNEL_GROUP group, std::string path, FMOD_MODE mode)
{
	m_sfx[sfx] = newd Sound();
	FMOD_RESULT result = m_system->createSound(std::string(SOUND_SFX_PATH + path).c_str(), mode, 0, &m_sfx[sfx]->data);
	m_sfx[sfx]->group = group;

	return result;
}

// Allocates a specific music piece into memory
FMOD_RESULT NoiseMachine::createSound(MUSIC music, CHANNEL_GROUP group, std::string path, FMOD_MODE mode)
{
	m_music[music] = newd Sound();
	FMOD_RESULT result = m_system->createSound(std::string(SOUND_MUSIC_PATH + path).c_str(), mode, 0, &m_music[music]->data);
	m_music[music]->group = group;

	return result;
}

// Altering the volume for all sounds in the group (Not optimized, but should only be called from the settings anyway.)
void NoiseMachine::setGroupVolume(CHANNEL_GROUP group, float inVolume)
{
	m_group[group]->setVolume(inVolume);
}

// Altering the pitch for all sounds in group (Not optimized, but should only be called from the settings anyway.)
void NoiseMachine::setGroupPitch(CHANNEL_GROUP group, float inPitch)
{
	m_group[group]->setPitch(inPitch);
}

// Returns the current group volume
float NoiseMachine::getGroupVolume(CHANNEL_GROUP group)
{
	float vol;
	ERRCHECK(m_group[group]->getVolume(&vol));
	return vol;
}

// Returns the current group pitch
float NoiseMachine::getGroupPitch(CHANNEL_GROUP group)
{
	float pitch;
	ERRCHECK(m_group[group]->getPitch(&pitch));
	return pitch;
}

// Prints information about sound
void NoiseMachine::DEBUG_SOUND(Sound* sound)
{
	if (sound->channel)
	{
		FMOD_RESULT		result;
		FMOD::Sound*	currentsound = 0;
		bool			playing = 0;
		bool			paused = 0;

		// Checks if channel is playing
		result = sound->channel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}

		// Checks if channel is pausing 
		result = sound->channel->getPaused(&paused);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}

		printf("FMOD_Info - IsPlaying: %d, IsPausing: %d, Current Sound Playing: %p.\n", playing, paused, sound->data);
	}
}

void NoiseMachine::CRASH_EVERYTHING(const char * format, ...)
{
	printf(format);
	exit(1);
}

void NoiseMachine::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
}