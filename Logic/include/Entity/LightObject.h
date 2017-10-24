#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include <Graphics\include\Structs.h>
#include <Misc\MapStructs.h>

#define LIGHT_DEBUG_BOX_SIZE 0.5f

namespace Graphics { class Renderer; }
namespace Logic
{
    class LightObject : public Graphics::Light
    {
    public:
        LightObject(FrameLight& frameLight);
        ~LightObject();

        void renderD(Graphics::Renderer& renderer);
        void render(Graphics::Renderer& renderer);

    private:
        Graphics::RenderDebugInfo renderDebug;
    };
}

#endif // !LIGHTOBJECT_H
