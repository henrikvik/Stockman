#pragma once
#include <Hybris\Hybris.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <map>

#include "Animation.h"

namespace HybrisLoader
{
    class Joint
    {
    public:
        Joint();
        Joint(Hybris::Joint & joint);
        DirectX::SimpleMath::Matrix invBindTransform;
    private:
    };

    class Skeleton
    {
    public:
        Skeleton::Skeleton(Hybris::Skeleton & skeleton);
        std::vector<SM::Matrix> evalAnimation(const char * animationName, float timeStamp);
        std::vector<SM::Matrix> getJointTransforms();

        float getAnimationDuration(const char * animationName) { return getAnimation(animationName)->getDuration(); }

    private:
        std::vector<Joint> joints;
        std::map<size_t, Animation> animations;

        constexpr size_t hash(const char * str) const;

        Animation * getAnimation(const char * animationName);
        std::vector<DirectX::SimpleMath::Matrix> evaluateJointTransforms(KeyFrame & keyFrame);
    };
}