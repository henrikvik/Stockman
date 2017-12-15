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
    float m_ambience;
	float m_SFX;
	float m_music;
	bool m_windowed;
    bool m_DOF;
    bool m_SSAO;
    bool m_fog;
    std::string m_name;
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
    float* getAmbiencePTR();
    float getAmbience();
    void setAmbience(float ambience);
    float* getSFXPTR();
    float getSFX();
    void setSFX(float SFX);
    float* getMusicPTR();
    float getMusic();
    void setMusic(float music);
    bool* getWindowedPTR();
    bool getWindowed();
    void setWindowed(bool windowed);
    bool* getDOFPTR();
    bool getDOF();
    void setDOF(bool DOF);
    bool* getSSAOPTR();
    bool getSSAO();
    void setSSAO(bool SSAO);
    bool* getFogPTR();
    bool getFog();
    void setFog(bool fog);
    std::string* getNamePTR();
    std::string getName();
    void setName(std::string name);
};

#endif