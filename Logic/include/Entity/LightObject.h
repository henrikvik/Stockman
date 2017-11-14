#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include <Graphics\include\RenderQueue.h>

#include <Misc\MapStructs.h>

namespace Graphics { class Renderer; }
namespace Logic
{
    class LightObject
    {
    public:
        LightObject(FrameLight& frameLight);
        ~LightObject();

        void renderD();
        void render() const;


        operator LightRenderInfo*() { return &renderInfo; }
    private:
        DebugRenderInfo debugRenderInfo;
        LightRenderInfo renderInfo;
    };
}

#endif // !LIGHTOBJECT_H
