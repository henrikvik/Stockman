#ifndef HUD_MANAGER_H
#define HUD_MANAGER_H

namespace Graphics
{
    struct HUDInfo;
}

namespace Logic
{
    class Player;
    class WaveTimeManager;
    class EntityManager;

    class HUDManager
    {
    private:
        Graphics::HUDInfo *info;
        static const int CURRENT_AMMO, TOTAL_AMMO;
    public:
        HUDManager();
        virtual ~HUDManager();

        void update(Player const &player, WaveTimeManager const &timeManager,
            EntityManager const &manager);
        void render();
    };
}

#endif