#include "Settings.h"
#include <Misc\FileLoader.h>
#include <vector>

Settings::Settings()
{
    m_mouseSense = 0.1f;
    m_FOV = 90.f;
    m_masterSound = 0.1f;;
    m_SFX = 0.1f;;
    m_music = 0.1f;
    m_windowed = false;
    m_name = "";
}

Settings::~Settings()
{
	
}

Settings& Settings::getInstance()
{
    static Settings instance;
	return instance;
}

void Settings::readFromFile()
{
	std::vector<Logic::FileLoader::LoadedStruct> loadTo;
	Logic::FileLoader::singleton().loadStructsFromFile(loadTo, "Settings");

	for (auto const& theSettings : loadTo)
	{
		if (theSettings.strings.at("Name").compare("MouseSense") == 0)
		{
			m_mouseSense = theSettings.floats.at("Value");
		}
		else if (theSettings.strings.at("Name").compare("FOV") == 0)
		{
			m_FOV = theSettings.floats.at("Value");
		}
		else if (theSettings.strings.at("Name").compare("MasterSound") == 0)
		{
			m_masterSound = theSettings.floats.at("Value");
		}
        else if (theSettings.strings.at("Name").compare("Ambience") == 0)
        {
            m_ambience = theSettings.floats.at("Value");
        }
		else if (theSettings.strings.at("Name").compare("SFX") == 0)
		{
			m_SFX = theSettings.floats.at("Value");
		}
		else if (theSettings.strings.at("Name").compare("Music") == 0)
		{
			m_music = theSettings.floats.at("Value");
		}
		else if (theSettings.strings.at("Name").compare("Windowed") == 0)
		{
			m_windowed = theSettings.floats.at("Value");
		}
        else if (theSettings.strings.at("Name").compare("PlayerName") == 0)
        {
            m_name = theSettings.strings.at("Value");
        }
	}
}

void Settings::writeToFile()
{
	std::vector<Logic::FileLoader::LoadedStruct> saveTo;
	int i = 0;
	Logic::FileLoader::LoadedStruct tempSave;
    Logic::FileLoader::LoadedStruct playerSave;

    playerSave.strings["Name"] = "PlayerName";
    playerSave.strings["Value"] = m_name;
    saveTo.push_back(playerSave);

	tempSave.strings["Name"] = "MouseSense";
	tempSave.floats["Value"] = m_mouseSense;
	saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "FOV";
	tempSave.floats["Value"] = m_FOV;
	saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "MasterSound";
	tempSave.floats["Value"] = m_masterSound;
	saveTo.push_back(tempSave);

    tempSave.strings["Name"] = "Ambience";
    tempSave.floats["Value"] = m_ambience;
    saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "SFX";
	tempSave.floats["Value"] = m_SFX;
	saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "Music";
	tempSave.floats["Value"] = m_music;
	saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "Windowed";
	tempSave.floats["Value"] = m_windowed;
	saveTo.push_back(tempSave);

	Logic::FileLoader::singleton().saveStructsToFile(saveTo, "Settings");

}

float* Settings::getMouseSensePTR()
{
	return &m_mouseSense;
}

float Settings::getMouseSense()
{
    return m_mouseSense;
}

void Settings::setMouseSense(float mouseSense)
{
	m_mouseSense = mouseSense;
}

float* Settings::getFOVPTR()
{
    return &m_FOV;
}

float Settings::getFOV()
{
	return m_FOV;
}

void Settings::setFOV(float FOV)
{
	m_FOV = FOV;
}

float* Settings::getMasterSoundPTR()
{
	return &m_masterSound;
}

float Settings::getMasterSound()
{
    return m_masterSound;
}

void Settings::setMasterSound(float masterSound)
{
	m_masterSound = masterSound;
}

float * Settings::getAmbiencePTR()
{
    return &m_ambience;
}

float Settings::getAmbience()
{
    return m_ambience;
}

void Settings::setAmbience(float ambience)
{
    m_ambience = ambience;
}

float* Settings::getSFXPTR()
{
	return &m_SFX;
}

float Settings::getSFX()
{
    return m_SFX;
}

void Settings::setSFX(float SFX)
{
	m_SFX = SFX;
}

float* Settings::getMusicPTR()
{
	return &m_music;
}

float Settings::getMusic()
{
    return m_music;
}

void Settings::setMusic(float music)
{
	m_music = music;
}

bool* Settings::getWindowedPTR()
{
	return &m_windowed;
}

bool Settings::getWindowed()
{
    return m_windowed;
}

void Settings::setWindowed(bool windowed)
{
	m_windowed = windowed;
}

std::string Settings::getName()
{
    return m_name;
}

void Settings::setName(std::string name)
{
    m_name = name;
}