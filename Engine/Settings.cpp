#include "Settings.h"
#include <Misc\FileLoader.h>
#include <vector>

Settings::Settings()
{

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
	}
}

void Settings::writeToFile()
{
	std::vector<Logic::FileLoader::LoadedStruct> saveTo;
	int i = 0;
	Logic::FileLoader::LoadedStruct tempSave;

	tempSave.strings["Name"] = "MouseSense";
	tempSave.floats["Value"] = m_mouseSense;
	saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "FOV";
	tempSave.floats["Value"] = m_FOV;
	saveTo.push_back(tempSave);

	tempSave.strings["Name"] = "MasterSound";
	tempSave.floats["Value"] = m_masterSound;
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
	return 90; /// GET FOV m_FOV;
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
