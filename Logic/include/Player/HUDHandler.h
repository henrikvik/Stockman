#ifndef HUD_HANDLER_H
#define HUD_HANDLER_H

#include <Graphics\include\Renderer.h>

namespace Graphics
{
    class Renderer;
    struct HUDInfo;
}

namespace Logic
{
    class Player;
    class WaveTimeManager;
    class EntityManager;

    class HUDHandler
    {
    private:
        Graphics::HUDInfo *info;
    public:
        HUDHandler();
        virtual ~HUDHandler();

        void update(Player const &player, WaveTimeManager const &timeManager,
            EntityManager const &manager);
        void render(Graphics::Renderer &renderer);
    };
}

#endif