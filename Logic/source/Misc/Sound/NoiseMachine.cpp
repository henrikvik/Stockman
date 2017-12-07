#include <Misc\Sound\NoiseMachine.h>
#include <Misc\Sound\SoundSource.h>
#include <Engine\Settings.h>

using namespace Sound;

const int NoiseMachine::THRESHOLD::SFX_MIN_DIST = 1;
const int NoiseMachine::THRESHOLD::SFX_MAX_DIST = 50;

// #define DEBUG_WRITE_INITIALIZING    // Print info about initializing
// #define DEBUG_WRITE_LOGGING      // Write in output log at exit
#define DEBUG_WRITE_ERRORS          // Write potential errors in console

#define __FILE__NO__PATH__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef DEBUG_WRITE_ERRORS
#define ERRCHECK(result) if (result != FMOD_OK) printf("FMOD error! (%d) %s, File: %s, Line: %d\n", result, FMOD_ErrorString(result), __FILE__NO__PATH__, __LINE__); 
#else
#define ERRCHECK(nothing)
#endif

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

    // Disable Logging
#ifdef DEBUG_WRITE_LOGGING
    FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG);
#else
    FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_NONE);
#endif

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
	initGroups(SOUNDSETTINGS::MUTE_ALL);
    //int nrSFX = initSFX(SOUNDSETTINGS::SFX_LOAD_MODE);    
    //int nrSongs = initMusic (SOUNDSETTINGS::MUSIC_LOAD_MODE);

    // Print information about noisemachine
#ifdef DEBUG_WRITE_INITIALIZING
    printf("Printing init debug for %s\n", __FILE__NO__PATH__);
    printf("Mute in NoiseStructs.h or in Settings at start menu.\n");
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
    printf(" \t(Loaded Songs/Ambient from: \t\t%s)\n", (SOUNDSETTINGS::MUSIC_LOAD_MODE) ? "Stream" : "Sample");
    printf("\n*Loaded Sounds/Ambient*\n%d/%d Sound Effects\n%d/%d Ambient/Songs\n", nrSFX, THRESHOLD::MAX_SFX, nrSongs, THRESHOLD::MAX_SONGS);
#endif
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

void Sound::NoiseMachine::clearCurrent()
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
}

// Prints out information about the current playing channels (Just for debugging, can be removed, will work without it)
void NoiseMachine::update(ListenerData& listener)
{
    try
    {
        if (listener.pos.x != listener.pos.x) throw std::exception("Nan vec");
	    // Sets the new listener "ears"
        ERRCHECK(m_system->set3DListenerAttributes(listener.id, &listener.pos, &listener.vel, &listener.forward, &listener.up));
	    // Update the 3D sound engine
	    ERRCHECK(m_system->update());
    }
    catch (std::exception const &ex)
    {
        printf("Sound error: %s\n", ex.what());
    }
}



// Stop all channels in each group
void NoiseMachine::stopAllGroups()
{
    for (int i = 0; i < THRESHOLD::MAX_GROUPS; i++) stopGroup(CHANNEL_GROUP(i));
}

// Stop a single group of noise
void NoiseMachine::stopGroup(CHANNEL_GROUP group)
{
   m_group[group]->stop();
}

// Overdrive should be true if global sfx (ex: Button presses)
void NoiseMachine::playSFX(SFX sfx, SoundSource* soundSource, bool overdrive)
{
    if (m_sfx[sfx])
	    checkIfPlay(m_sfx[sfx], soundSource, overdrive);
}

// Overdrive should be true if global music (ex: Main-Menu Music)
void NoiseMachine::playMusic(MUSIC music, SoundSource* soundSource, bool overdrive)
{
    if (m_music[music])
	    checkIfPlay(m_music[music], soundSource, overdrive);
}

// Checks if sound if currently playing and if overdrive should be used
void NoiseMachine::checkIfPlay(Noise* sound, SoundSource* soundSource, bool overdrive)
{
    if (soundSource && !overdrive)
    {
        printf("Error: Soundsource can't be null and a overdrive = false!");
        return;
    }

	if (overdrive)
		play(sound, soundSource);
	else
	{
		// Checking if channel is currently playing something
		static bool playing = false;
		if (soundSource->channel)
		{
			soundSource->channel->isPlaying(&playing);
		}

		// If not playing sound, play sound
		if (!playing)
			play(sound, soundSource);
	}
}

// Actually plays the sound
void NoiseMachine::play(Noise* sound, SoundSource* soundSource)
{
    // Special case for global music without soundsource
    if (!soundSource)
    {
        ERRCHECK(m_system->playSound(sound->data, m_group[sound->group], false, &sound->channel));
        ERRCHECK(m_system->update());
        return;
    }

	// Playing sound
	ERRCHECK(m_system->playSound(sound->data, m_group[sound->group], false, &soundSource->channel));
    
    // Force update (because FMOD updates faster than our game)
    soundSource->channel->set3DAttributes(&soundSource->pos, &soundSource->vel);
    ERRCHECK(m_system->update());

	// Saving the created channel onto the sound class for modifications on it
	sound->channel = soundSource->channel;
}

// Initialize all sound groups
void NoiseMachine::initGroups(bool mute)
{
	// Setting speicific groups
	ERRCHECK(m_system->createChannelGroup("SFX",		&m_group[CHANNEL_GROUP::CHANNEL_SFX]));
	ERRCHECK(m_system->createChannelGroup("MUSIC",		&m_group[CHANNEL_GROUP::CHANNEL_MUSIC]));
	ERRCHECK(m_system->createChannelGroup("AMBIENT",	&m_group[CHANNEL_GROUP::CHANNEL_AMBIENT]));

	// Setting master group
	ERRCHECK(m_system->getMasterChannelGroup(&m_group[CHANNEL_GROUP::CHANNEL_MASTER]));

    // Muting if active
    if (mute) for (int i = 0; i < THRESHOLD::MAX_GROUPS; i++) m_group[i]->setVolume(NULL);
    else
    {
        Settings& setting = Settings::getInstance();
        // Otherwise, set default volumes
        m_group[CHANNEL_GROUP::CHANNEL_MASTER]->setVolume(setting.getMasterSound());
        m_group[CHANNEL_GROUP::CHANNEL_AMBIENT]->setVolume(setting.getAmbience());
        m_group[CHANNEL_GROUP::CHANNEL_MUSIC]->setVolume(setting.getMusic());
        m_group[CHANNEL_GROUP::CHANNEL_SFX]->setVolume(setting.getSFX());
    }
}

// Initialize all sound effects
int NoiseMachine::initSFX(LOAD_MODE loadMode)
{
	// Init all the sfx here
    ERRCHECK(createSound(loadMode, SFX::FOOTSTEP_SNOW, CHANNEL_GROUP::CHANNEL_SFX, "Footstep_Snow.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::FOOTSTEP_SMALL, CHANNEL_GROUP::CHANNEL_SFX, "Footstep_Small.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::JUMP, CHANNEL_GROUP::CHANNEL_SFX, "Jump.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_CUTLERY_PRIMARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Cutlery_1.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_CUTLERY_SECONDARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Cutlery_2.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_ICEGUN_PRIMARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Ice_1.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_ICEGUN_SECONDARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Ice_2.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_ICEGUN_THIRD, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Ice_3.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MELEE_PRIMARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Melee_Swing.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MELEE_SECONDARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Melee_Parry.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_RELOAD, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Reload.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::NECROMANCER_DEATH, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Laugh.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::NECROMANCER_SPAWN, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Spawn.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SWOOSH, CHANNEL_GROUP::CHANNEL_SFX, "Swoosh.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_1, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability1.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_2, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability2.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_3, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability3.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_4, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability4.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_5, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability5.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_6, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability6.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_MELEE_USE, CHANNEL_GROUP::CHANNEL_SFX, "boss1MeleeUse.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_GRAPPLING, CHANNEL_GROUP::CHANNEL_SFX, "Skill_Grappling.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_BULLETTIME_HEART, CHANNEL_GROUP::CHANNEL_SFX, "Skill_BulletTime_Heart.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_BULLETTIME_TIME, CHANNEL_GROUP::CHANNEL_SFX, "Skill_BulletTime_Time.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_CHARGE, CHANNEL_GROUP::CHANNEL_SFX, "Skill_SheildCharge.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::HELLO, CHANNEL_GROUP::CHANNEL_SFX, "hello.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_BUTTON_PRESS, CHANNEL_GROUP::CHANNEL_SFX, "ui_button_press.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_BUTTON_HOVER, CHANNEL_GROUP::CHANNEL_SFX, "ui_button_hover.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::TRIGGER_PICKUP, CHANNEL_GROUP::CHANNEL_SFX, "Trigger_Pickup.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::TRIGGER_JUMPPAD, CHANNEL_GROUP::CHANNEL_SFX, "Trigger_Jumppad.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::START_GAME, CHANNEL_GROUP::CHANNEL_SFX, "start.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_DEATH, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Death.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_HIT, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Hit.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_AMBIENT_1, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Ambient_1.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_AMBIENT_2, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Ambient_2.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MAGIC_1, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Magic_1.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MAGIC_2, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Magic_2.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UPGRADE_UNLOCKED, CHANNEL_GROUP::CHANNEL_SFX, "Upgrade_Unlocked.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::CAMPFIRE, CHANNEL_GROUP::CHANNEL_SFX, "Campfire.ogg", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, SFX::WAVE_START, CHANNEL_GROUP::CHANNEL_SFX, "Wave_Start.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WAVE_END, CHANNEL_GROUP::CHANNEL_SFX, "Wave_End.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WAVE_CARD, CHANNEL_GROUP::CHANNEL_SFX, "Wave_Card.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WAVE_DEAD, CHANNEL_GROUP::CHANNEL_SFX, "Wave_Dead.ogg", FMOD_2D));

	// Setting the thresholds of where the listener can hear the sfx
    int count = 0;
    for (int i = 0; i < THRESHOLD::MAX_SFX; i++)
    {
        if (m_sfx[i])
        {
            count++;
            m_sfx[i]->data->set3DMinMaxDistance((float)THRESHOLD::SFX_MIN_DIST, (float)THRESHOLD::SFX_MAX_DIST);
        }
    }

    return count; 
}

// Initialize all music pieces
int NoiseMachine::initMusic(LOAD_MODE loadMode)
{
	// Init all the music here
	ERRCHECK(createSound(loadMode, MUSIC::MUSIC_MAIN_MENU, CHANNEL_GROUP::CHANNEL_MUSIC, "stockman.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::MUSIC_IN_GAME, CHANNEL_GROUP::CHANNEL_MUSIC, "beyond.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::MUSIC_CREDITS, CHANNEL_GROUP::CHANNEL_MUSIC, "lab.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::AMBIENT_STORM, CHANNEL_GROUP::CHANNEL_AMBIENT, "ambient_snow.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::BOSS_1_MUSIC_1, CHANNEL_GROUP::CHANNEL_MUSIC, "boss1theme1.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::BOSS_1_MUSIC_2, CHANNEL_GROUP::CHANNEL_MUSIC, "boss1theme2.ogg", FMOD_2D | FMOD_LOOP_NORMAL));

	// Setting the thresholds of where the listener can hear the music
    int count = 0;
    for (int i = 0; i < THRESHOLD::MAX_SONGS; i++)
    {
        if (m_music[i])
        {
            count++;
            m_music[i]->data->set3DMinMaxDistance((float)THRESHOLD::MUSIC_MIN_DIST, (float)THRESHOLD::MUSIC_MAX_DIST);
        }
    }

    return count;
}

int Sound::NoiseMachine::loadMenuSounds()
{
    LOAD_MODE loadMode = SOUNDSETTINGS::MUSIC_LOAD_MODE;
    int totalCount = 0;
    // Init all the music here
    ERRCHECK(createSound(loadMode, MUSIC::MUSIC_MAIN_MENU, CHANNEL_GROUP::CHANNEL_MUSIC, "stockman.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::AMBIENT_STORM, CHANNEL_GROUP::CHANNEL_AMBIENT, "ambient_snow.ogg", FMOD_2D | FMOD_LOOP_NORMAL));


    // Setting the thresholds of where the listener can hear the music
    int count = 0;
    for (int i = 0; i < THRESHOLD::MAX_SONGS; i++)
    {
        if (m_music[i])
        {
            count++;
            m_music[i]->data->set3DMinMaxDistance((float)THRESHOLD::MUSIC_MIN_DIST, (float)THRESHOLD::MUSIC_MAX_DIST);
        }
    }

    totalCount += count;

    loadMode = SOUNDSETTINGS::SFX_LOAD_MODE;
    // Init all the sfx here
    ERRCHECK(createSound(loadMode, SFX::HELLO, CHANNEL_GROUP::CHANNEL_SFX, "hello.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_BUTTON_PRESS, CHANNEL_GROUP::CHANNEL_SFX, "ui_button_press.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_BUTTON_HOVER, CHANNEL_GROUP::CHANNEL_SFX, "ui_button_hover.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_SAND_FX, CHANNEL_GROUP::CHANNEL_SFX, "ui_dust_fx.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::START_GAME, CHANNEL_GROUP::CHANNEL_SFX, "start.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::CAMPFIRE, CHANNEL_GROUP::CHANNEL_SFX, "Campfire.ogg", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL));

    // Setting the thresholds of where the listener can hear the sfx
    count = 0;
    for (int i = 0; i < THRESHOLD::MAX_SFX; i++)
    {
        if (m_sfx[i])
        {
            count++;
            m_sfx[i]->data->set3DMinMaxDistance((float)THRESHOLD::SFX_MIN_DIST, (float)THRESHOLD::SFX_MAX_DIST);
        }
    }

    totalCount += count;


    return totalCount;
}

int Sound::NoiseMachine::loadPlaySounds()
{

    LOAD_MODE loadMode = SOUNDSETTINGS::MUSIC_LOAD_MODE;
    int totalCount = 0;
    // Init all the music here
    ERRCHECK(createSound(loadMode, MUSIC::MUSIC_IN_GAME, CHANNEL_GROUP::CHANNEL_MUSIC, "beyond.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::MUSIC_CREDITS, CHANNEL_GROUP::CHANNEL_MUSIC, "lab.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::AMBIENT_STORM, CHANNEL_GROUP::CHANNEL_AMBIENT, "ambient_snow.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::BOSS_1_MUSIC_1, CHANNEL_GROUP::CHANNEL_MUSIC, "boss1theme1.ogg", FMOD_2D | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, MUSIC::BOSS_1_MUSIC_2, CHANNEL_GROUP::CHANNEL_MUSIC, "boss1theme2.ogg", FMOD_2D | FMOD_LOOP_NORMAL));


    // Setting the thresholds of where the listener can hear the music
    int count = 0;
    for (int i = 0; i < THRESHOLD::MAX_SONGS; i++)
    {
        if (m_music[i])
        {
            count++;
            m_music[i]->data->set3DMinMaxDistance((float)THRESHOLD::MUSIC_MIN_DIST, (float)THRESHOLD::MUSIC_MAX_DIST);
        }
    }

    totalCount += count;
    loadMode = SOUNDSETTINGS::SFX_LOAD_MODE;

    // Init all the sfx here
    ERRCHECK(createSound(loadMode, SFX::FOOTSTEP_SNOW, CHANNEL_GROUP::CHANNEL_SFX, "Footstep_Snow.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::FOOTSTEP_SMALL, CHANNEL_GROUP::CHANNEL_SFX, "Footstep_Small.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::JUMP, CHANNEL_GROUP::CHANNEL_SFX, "Jump.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_CUTLERY_PRIMARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Cutlery_1.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_CUTLERY_SECONDARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Cutlery_2.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_ICEGUN_PRIMARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Ice_1.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_ICEGUN_SECONDARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Ice_2.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_ICEGUN_THIRD, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Ice_3.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MELEE_PRIMARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Melee_Swing.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MELEE_SECONDARY, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Melee_Parry.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_RELOAD, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Reload.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::NECROMANCER_DEATH, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Laugh.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::NECROMANCER_SPAWN, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Spawn.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SWOOSH, CHANNEL_GROUP::CHANNEL_SFX, "Swoosh.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_1, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability1.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_2, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability2.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_3, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability3.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_4, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability4.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_5, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability5.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_ABILITY_6, CHANNEL_GROUP::CHANNEL_SFX, "boss1Ability6.mp3", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::BOSS_1_MELEE_USE, CHANNEL_GROUP::CHANNEL_SFX, "boss1MeleeUse.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_GRAPPLING, CHANNEL_GROUP::CHANNEL_SFX, "Skill_Grappling.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_BULLETTIME_HEART, CHANNEL_GROUP::CHANNEL_SFX, "Skill_BulletTime_Heart.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_BULLETTIME_TIME, CHANNEL_GROUP::CHANNEL_SFX, "Skill_BulletTime_Time.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::SKILL_CHARGE, CHANNEL_GROUP::CHANNEL_SFX, "Skill_SheildCharge.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_BUTTON_PRESS, CHANNEL_GROUP::CHANNEL_SFX, "ui_button_press.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_BUTTON_HOVER, CHANNEL_GROUP::CHANNEL_SFX, "ui_button_hover.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UI_SAND_FX, CHANNEL_GROUP::CHANNEL_SFX, "ui_dust_fx.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::TRIGGER_PICKUP, CHANNEL_GROUP::CHANNEL_SFX, "Trigger_Pickup.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::TRIGGER_JUMPPAD, CHANNEL_GROUP::CHANNEL_SFX, "Trigger_Jumppad.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_DEATH, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Death.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_HIT, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Hit.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_AMBIENT_1, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Ambient_1.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::ENEMY_AMBIENT_2, CHANNEL_GROUP::CHANNEL_SFX, "Enemy_Ambient_2.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MAGIC_1, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Magic_1.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::WEAPON_MAGIC_2, CHANNEL_GROUP::CHANNEL_SFX, "Weapon_Magic_2.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::UPGRADE_UNLOCKED, CHANNEL_GROUP::CHANNEL_SFX, "Upgrade_Unlocked.ogg", FMOD_3D_LINEARROLLOFF));
    ERRCHECK(createSound(loadMode, SFX::CAMPFIRE, CHANNEL_GROUP::CHANNEL_SFX, "Campfire.ogg", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL));
    ERRCHECK(createSound(loadMode, SFX::WAVE_START, CHANNEL_GROUP::CHANNEL_SFX, "Wave_Start.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WAVE_END, CHANNEL_GROUP::CHANNEL_SFX, "Wave_End.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WAVE_CARD, CHANNEL_GROUP::CHANNEL_SFX, "Wave_Card.ogg", FMOD_2D));
    ERRCHECK(createSound(loadMode, SFX::WAVE_DEAD, CHANNEL_GROUP::CHANNEL_SFX, "Wave_Dead.ogg", FMOD_2D));

    // Setting the thresholds of where the listener can hear the sfx
    count = 0;
    for (int i = 0; i < THRESHOLD::MAX_SFX; i++)
    {
        if (m_sfx[i])
        {
            count++;
            m_sfx[i]->data->set3DMinMaxDistance((float)THRESHOLD::SFX_MIN_DIST, (float)THRESHOLD::SFX_MAX_DIST);
        }
    }

    totalCount += count;


    return totalCount;
}

// Allocates a specific sound effect into memory or stream
FMOD_RESULT NoiseMachine::createSound(LOAD_MODE loadMode, SFX sfx, CHANNEL_GROUP group, std::string path, FMOD_MODE mode)
{
	m_sfx[sfx] = newd Noise();
    FMOD_RESULT result;
    switch (loadMode) {
    case STREAM: result = m_system->createStream(std::string(SOUND_SFX_PATH + path).c_str(), mode, 0, &m_sfx[sfx]->data);  break;
    case SAMPLE: result = m_system->createSound (std::string(SOUND_SFX_PATH + path).c_str(), mode, 0, &m_sfx[sfx]->data);  break;  }
    ERRCHECK(result);
	m_sfx[sfx]->group = group;

	return result;
}

// Allocates a specific music piece into memory or stream
FMOD_RESULT NoiseMachine::createSound(LOAD_MODE loadMode, MUSIC music, CHANNEL_GROUP group, std::string path, FMOD_MODE mode)
{
	m_music[music] = newd Noise();
    FMOD_RESULT result;
    switch (loadMode) {
    case STREAM: result = m_system->createStream(std::string(SOUND_MUSIC_PATH + path).c_str(), mode, 0, &m_music[music]->data);     break;
    case SAMPLE: result = m_system->createSound (std::string(SOUND_MUSIC_PATH + path).c_str(), mode, 0, &m_music[music]->data);     break;  }
    ERRCHECK(result);
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
