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
    bool sledge;
    float cd[2];
    UINT score;
    int wave;
    int enemiesRemaining;
    float timeRemaining;
    int currentWeapon;
    int currentSkills[2];
};

namespace Logic
{
    class Player;
    class WaveTimeManager;
    class EntityManager;

    class HUDManager
    {
    private:
        GUIInfo info;
        bool skillChoosen;

        static const int CURRENT_AMMO, TOTAL_AMMO;

        std::vector<Sprite> HUDElements;
        std::vector<Sprite> skillList;
        std::vector<Sprite> skillMasks;
        std::vector<Sprite> HPBar;
        std::vector<Sprite> staticElements;



        void constructGUIElements();
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

        
    public:
        HUDManager();
        virtual ~HUDManager();

        void update(Player const &player, WaveTimeManager const &timeManager,
            EntityManager const &manager);
        void render() const;
        void reset();
    };
}

#endif