#ifndef HUD_MANAGER_H
#define HUD_MANAGER_H
#include <Graphics\include\RenderQueue.h>

namespace Graphics
{
    struct HUDInfo;
}

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
        SpriteRenderInfo GUIElements[NROFICONS];
        //all possible icons for your choosen skills. off cd / on cd
        SpriteRenderInfo choosenSKills[4];
        void constructGUIElements();
        void updateGUIElemets();
        
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