#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
private:
	static Settings instance;
	float m_mouseSense;
	float m_FOV;
	float m_masterSound;
	float m_SFX;
	float m_music;
	bool m_windowed;
    std::string m_alias;
public:
	Settings();
	~Settings();
	static Settings& getInstance();
	void readFromFile();
	void writeToFile();
	float* getMouseSensePTR();
    float getMouseSense();
	void setMouseSense(float mouseSense);
	float* getFOVPTR();
    float getFOV();
	void setFOV(float FOV);
	float* getMasterSoundPTR();
    float getMasterSound();
	void setMasterSound(float masterSound);
	float* getSFXPTR();
    float getSFX();
	void setSFX(float SFX);
	float* getMusicPTR();
    float getMusic();
	void setMusic(float music);
	bool* getWindowedPTR();
    bool getWindowed();
	void setWindowed(bool windowed);
    std::string getAlias();
    void setAlias(std::string alias);
};

#endif