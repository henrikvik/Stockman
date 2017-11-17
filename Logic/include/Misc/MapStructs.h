#ifndef MAPSTRUCTS
#define MAPSTRUCTS

#include <btBulletCollisionCommon.h>
#include <Resources\Resources.h>

namespace Logic
{
    struct FrameHitbox {
        btVector3 position;
        btVector3 rotation;
        btVector3 dimensions;
        Resources::Models::Files modelID;  // Should get removed at later stage
    };

    struct FrameProp {
        btVector3 position;
        btVector3 rotation;
        Resources::Models::Files modelID;
    };

    struct FrameLight {
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector3 color;
        float intensity;
        float range;
    };
}

#endif // !MAPSTRUCTS