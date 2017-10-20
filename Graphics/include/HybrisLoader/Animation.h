#pragma once
#include <Hybris\Hybris.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <map>

namespace HybrisLoader
{
    class JointTransform
    {
    public:
        static JointTransform lerp(JointTransform & from, JointTransform & to, float progress);

        JointTransform();
        JointTransform(Hybris::JointTransform & jointTransform);

        SM::Matrix createMatrix();

        bool operator==(JointTransform const& other) const;
    private:
        DirectX::SimpleMath::Vector3 translation;
        DirectX::SimpleMath::Quaternion rotation;
        DirectX::SimpleMath::Vector3 scale;
    };

    class KeyFrame
    {
    public:
        static KeyFrame lerp(KeyFrame & from, KeyFrame & to, float progress);

        KeyFrame();
        KeyFrame(Hybris::KeyFrame & keyFrame);
        
        float timeStamp;
        std::vector<JointTransform> jointTransforms;
    };

    class Animation
    {
    public:
        Animation(Hybris::Animation & animation);
        std::pair<KeyFrame*, KeyFrame*> getCurrentAndNextFrame(float timeStamp);

        float getDuration();
    private:
        std::string name;
        float duration;
        std::vector<KeyFrame> keyFrames;
    };
}
