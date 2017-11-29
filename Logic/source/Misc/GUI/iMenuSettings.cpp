#include "..\..\..\include\Misc\GUI\iMenuSettings.h"
#include <Misc\Sound\NoiseMachine.h>

Logic::iMenuSettings::iMenuSettings(iMenu::MenuGroup group)
    : iMenu(group)
{
}

Logic::iMenuSettings::~iMenuSettings()
{
    confirm();
}

void Logic::iMenuSettings::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);


}

void Logic::iMenuSettings::render() const
{
    iMenu::render();
}

void Logic::iMenuSettings::confirm()
{
    for (Slider& sld : m_sliders)
    {
        sld.confirm();
        std::cout << sld.getName();
        if (sld.getName().compare("FOVSlider") == 0)
        {
            Global::mainCamera->updateFOV(sld.getValue());
            std::cout << "Entered FOV";
        }
        else if (sld.getName().compare("MasterSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_MASTER, sld.getValue());
        }
        else if (sld.getName().compare("MusicSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_MUSIC, sld.getValue());
        }
        else if (sld.getName().compare("AmbienceSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_AMBIENT, sld.getValue());
        }
        else if (sld.getName().compare("SFXSlider") == 0)
        {
            Sound::NoiseMachine::Get().setGroupVolume(Sound::CHANNEL_GROUP::CHANNEL_SFX, sld.getValue());
        }
    }
}
