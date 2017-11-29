#ifndef HUD_MANAGER_H
#define HUD_MANAGER_H
#include <Graphics\include\RenderQueue.h>
#include "../Misc/GUI/Sprite.h"
#include "../Misc/GUI/Button.h"

//all possible GUI elemets
#define NROFICONS 12

//all gui elemts that should be drawn
#define USEDGUIELEMTS 8

struct GUIInfo
{
    int hp;
    int activeAmmo[2];
    int inactiveAmmo[2];
    int cdInSeconds[2];
    int wave, maxWaves;
    int enemiesRemaining;
    int currentWeapon;
    int currentSkills[2];

    float cd[2];
    float timeRemaining;

    bool sledge;
    UINT score, scoreCombo, scoreMul;
    std::wstring waveText;
};

namespace Logic
{
    class Player;
    class WaveTimeManager;
    class EntityManager;

    class HUDManager
    {
    private:
        static const std::wstring IN_WAVE, BEFORE_WAVE, AFTER_WAVE;

        GUIInfo info;
        bool skillChoosen;

        static const int CURRENT_AMMO, TOTAL_AMMO;

        std::vector<Sprite> HUDElements;
        std::vector<Sprite> skillList;
        std::vector<Sprite> skillMasks;
        std::vector<Sprite> HPBar;
        std::vector<Sprite> staticElements;
        std::vector<TextRenderInfo> HUDText;
        std::vector<std::wstring> liveText;
        std::vector<Sprite> waveSprites;

        bool showWaveCleared;

        void constructGUIElements();
        void updateTextElements();
        void updateGUIElemets();
        enum Elemnts
        {
            CROSSHAIR,
            CROSSBOW,
            ICESTAFF,
            SLEDGEHAMMER,
            WEAPONMASK,
            SKILL1,
            SKILL2,
            SKILLMASK1,
            SKILLMASK2,
        };
        void renderTextElements()const;
    public:
        HUDManager();
        virtual ~HUDManager();

        void update(Player const &player, WaveTimeManager const &timeManager,
            EntityManager const &manager, float dt);
        void render() const;
        void reset();
    };
}

#endif