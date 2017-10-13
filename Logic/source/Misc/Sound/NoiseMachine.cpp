#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

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
	ERRCHECK(m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, extraDriverData));
	ERRCHECK(result);

	// Initialize sounds
	initSFX();
	initMusic();
}

void NoiseMachine::update()
{
	if (m_channel)
	{
		FMOD_RESULT		result;
		FMOD::Sound*	currentsound	= 0;
		bool			playing			= 0;
		bool			paused			= 0;

		// Checks if channel is playing
		result = m_channel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}

		// Checks if channel is pausing 
		result = m_channel->getPaused(&paused);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}

		// Gets the current playing sound
		ERRCHECK(m_channel->getCurrentSound(&currentsound));

		printf("FMOD_Info - IsPlaying: %d, IsPausing: %d, Current Sound Playing: %p.\n", playing, paused, currentsound);
	}
}

void NoiseMachine::playSFX(SoundData sData, SFX sfx, ...)
{
	ERRCHECK(m_system->playSound(m_sound[sfx], 0, false, &m_channel));
}

void NoiseMachine::playMusic(SoundData sData, MUSIC music, ...)
{
	ERRCHECK(m_system->playSound(m_music[music], 0, false, &m_channel));
}

void NoiseMachine::initSFX()
{
	ERRCHECK(createSound(SFX::TEST, "test.ogg"));
}

void NoiseMachine::initMusic()
{
}

FMOD_RESULT NoiseMachine::createSound(SFX sfx, std::string path)
{
	return m_system->createSound(std::string(SOUND_SFX_PATH + path).c_str(), FMOD_DEFAULT, 0, &m_sound[sfx]);
}

FMOD_RESULT NoiseMachine::createSound(MUSIC music, std::string path)
{
	return m_system->createSound(std::string(SOUND_MUSIC_PATH + path).c_str(), FMOD_DEFAULT, 0, &m_music[music]);
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