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
        FrameLight(DirectX::SimpleMath::Vector3 inPosition, DirectX::SimpleMath::Vector3 inColor, float inIntensity, float inRange) 
            : position(inPosition), color(inColor), intensity(inIntensity), range(inRange) { }
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector3 color;
        float intensity;
        float range;
    };
}

#endif // !MAPSTRUCTS