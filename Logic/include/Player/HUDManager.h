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
    int ammoPickedUp;

    float cd[2];
    float timeRemaining;

    int comboTimeRemaining;


    bool sledge, isReloding;
    UINT score, scoreCombo, scoreMul;
    std::wstring waveText;
};

namespace Logic
{
    class Player;
    class WaveTimeManager;
    class EntityManager;
    class iMenuFX;

    class HUDManager
    {
    private:
        static const std::wstring IN_WAVE, BEFORE_WAVE, AFTER_WAVE;

        GUIInfo info;
        bool skillChoosen;

        static const int CURRENT_AMMO, TOTAL_AMMO;

        static const float WAVE_SLIDE_TIME, ENRAGE_SLIDE_TIME, PICKEUP_MESSAGE_TIMER, COMBO_BAR_WIDTH;


        std::vector<Sprite> HUDElements;
        std::vector<Sprite> skillList;
        std::vector<Sprite> skillMasks;
        std::vector<Sprite> HPBar;
        std::vector<Sprite> comboBar;
        std::vector<Sprite> staticElements;
        std::vector<TextRenderInfo> HUDText;
        std::vector<Sprite> waveSprites;
        std::vector<iMenuFX*> effects;

        bool showWaveCleared;
        float nextWaveSlideTimer;
        float enrageSlideTimer;
        bool wasEnraged;
        bool cards;

        float crossBowTimer;
        float staffTimer;

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
            SKILLMASK2
        };
        enum WaveMessages
        {
            NEXTWAVE,
            ENRAGE
        };
        void renderTextElements()const;
    public:
        HUDManager();
        virtual ~HUDManager();

        void update(Player &player, WaveTimeManager const &timeManager,
            EntityManager const &manager, float dt, bool cards = false);
        void render() const;
        void reset();
    };
}

#endif