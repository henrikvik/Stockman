#include <Misc\Sound\NoiseMachine.h>
#include <Misc\Sound\SoundSource.h>

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

    // Setting full array as nullptr to avoid crashing
    for (int i = 0; i < THRESHOLD::MAX_SFX; i++)    m_sfx[i]    = nullptr;
    for (int i = 0; i < THRESHOLD::MAX_SONGS; i++)  m_music[i]  = nullptr;

	// Initialize sounds
	initGroups();
    int nrSFX = initSFX(SOUNDSETTINGS::SFX_LOAD_MODE);    
    int nrSongs = initMusic (SOUNDSETTINGS::MUSIC_LOAD_MODE);

    // Print information about noisemachine
    int numdrivers, systemrate, speakermodechannels;
    long long samplebytes, streambytes, otherbytes;
    FMOD_SPEAKERMODE speakermode;
    FMOD_GUID guid;
    m_system->getNumDrivers(&numdrivers);
    m_system->getDriverInfo(0, 0, 0, &guid, &systemrate, &speakermode, &speakermodechannels);
    m_system->getFileUsage(&samplebytes, &streambytes, &otherbytes);
    printf("*Sound information*\nNumber of sound drivers: \t%d\nSpeaker Mode: \t\t\t%d\nSpeaker Mode Channels: \t\t%d\nSound Sample Rate: \t\t%d\n", numdrivers, speakermode, speakermodechannels, systemrate);
    printf("\n*Sound usage in bytes*\nSamples: \t\t\t%lld\nStream: \t\t\t%lld\nOther: \t\t\t\t%lld\n", samplebytes, streambytes, otherbytes);
    printf(" \t(Loaded Sound Effects from: \t%s)\n", (SOUNDSETTINGS::SFX_LOAD_MODE) ? "Stream" : "Sample");
    printf(" \t(Loaded Songs from: \t\t%s)\n", (SOUNDSETTINGS::MUSIC_LOAD_MODE) ? "Stream" : "Sample");
    printf("\n*Loaded Sounds*\n%d/%d Sound Effects\n%d/%d Songs\n", nrSFX, THRESHOLD::MAX_SFX, nrSongs, THRESHOLD::MAX_SONGS);
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
}

void NoiseMachine::playSFX(SFX sfx, SoundSource* soundSource, bool overdrive)
{
    if (m_sfx[sfx])
	    checkIfPlay(m_sfx[sfx], soundSource, overdrive);
}

void NoiseMachine::playMusic(MUSIC music, SoundSource* soundSource, bool overdrive)
{
    if (m_music[music])
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
int NoiseMachine::initSFX(LOAD_MODE loadMode)
{
	// Init all the sfx here
    int count = 1; // Just for debugging purposes, won't crash anything
    ERRCHECK(createSound(loadMode, SFX::BOING, CHANNEL_GROUP::CHANNEL_SFX, "test.ogg", FMOD_3D_LINEARROLLOFF));

	// Setting the thresholds of where the listener can hear the sfx
	for (int i = 0; i < THRESHOLD::MAX_SFX; i++)
		if (m_sfx[i])
			m_sfx[i]->data->set3DMinMaxDistance(THRESHOLD::SFX_MIN_DIST, THRESHOLD::SFX_MAX_DIST);

    return count;
}

// Initialize all music pieces
int NoiseMachine::initMusic(LOAD_MODE loadMode)
{
	// Init all the music here
    int count = 2; // Just for debugging purposes, won't crash anything
	ERRCHECK(createSound(loadMode, MUSIC::ENRAGE, CHANNEL_GROUP::CHANNEL_MUSIC, "enrageTimer.mp3", FMOD_3D));
	ERRCHECK(createSound(loadMode, MUSIC::TEST_MUSIC, CHANNEL_GROUP::CHANNEL_MUSIC, "music.ogg", FMOD_3D_LINEARROLLOFF));

	// Setting the thresholds of where the listener can hear the music
	for (int i = 0; i < THRESHOLD::MAX_SONGS; i++)
		if (m_sfx[i])
			m_music[i]->data->set3DMinMaxDistance(THRESHOLD::MUSIC_MIN_DIST, THRESHOLD::MUSIC_MAX_DIST);

    return count;
}

// Allocates a specific sound effect into memory or stream
FMOD_RESULT NoiseMachine::createSound(LOAD_MODE loadMode, SFX sfx, CHANNEL_GROUP group, std::string path, FMOD_MODE mode)
{
	m_sfx[sfx] = newd Sound();
    FMOD_RESULT result;
    switch (loadMode) {
    case STREAM: result = m_system->createStream(std::string(SOUND_SFX_PATH + path).c_str(), mode, 0, &m_sfx[sfx]->data);  break;
    case SAMPLE: result = m_system->createSound (std::string(SOUND_SFX_PATH + path).c_str(), mode, 0, &m_sfx[sfx]->data);  break;  }
	m_sfx[sfx]->group = group;

	return result;
}

// Allocates a specific music piece into memory or stream
FMOD_RESULT NoiseMachine::createSound(LOAD_MODE loadMode, MUSIC music, CHANNEL_GROUP group, std::string path, FMOD_MODE mode)
{
	m_music[music] = newd Sound();
    FMOD_RESULT result;
    switch (loadMode) {
    case STREAM: result = m_system->createStream(std::string(SOUND_MUSIC_PATH + path).c_str(), mode, 0, &m_music[music]->data);     break;
    case SAMPLE: result = m_system->createSound (std::string(SOUND_MUSIC_PATH + path).c_str(), mode, 0, &m_music[music]->data);     break;  }
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
