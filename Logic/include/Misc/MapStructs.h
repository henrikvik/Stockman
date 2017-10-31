#ifndef MAPSTRUCTS
#define MAPSTRUCTS

#include <btBulletCollisionCommon.h>

namespace Logic
{
    struct FrameHitbox {
        btVector3 position;
        btVector3 rotation;
        btVector3 dimensions;
        Graphics::ModelID modelID;  // Should get removed at later stage
    };

    struct FrameProp {
        btVector3 position;
        btVector3 rotation;
        Graphics::ModelID modelID;
    };

    struct FrameLight {
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector3 color;
        float intensity;
        float range;
    };
}

#endif // !MAPSTRUCTS