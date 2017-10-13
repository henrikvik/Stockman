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
	result = m_system->createSound(SOUND_SFX_PATH("test.wav"), FMOD_DEFAULT, 0, &m_sound);
	ERRCHECK(result);
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

		printf("FMOD_Info - IsPlaying: %d, IsPausing: %d, Current Sound Playing: %d.\n", playing, paused, currentsound);
	}
}

void NoiseMachine::playSound()
{
	ERRCHECK(m_system->playSound(m_sound, 0, false, &m_channel));
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