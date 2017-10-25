#include "Animation.h"

namespace HybrisLoader
{
    Animation::Animation(Hybris::Animation & animation)
    {
        name = animation.name;
        duration = animation.duration;
        for (size_t i = 0; i < animation.keyFrames.size; i++)
        {
            keyFrames.emplace_back(animation.keyFrames[i]);
        }
    }

    std::pair<KeyFrame*, KeyFrame*> Animation::getCurrentAndNextFrame(float timeStamp)
    {
        int prevKeyFrame = 0;
        int nextKeyFrame = 0;

        for (int i = 0, size = keyFrames.size(); i < size; i++)
        {
            if (keyFrames[i].timeStamp < timeStamp)
            {
                prevKeyFrame = i;
                nextKeyFrame = min(i + 1, size - 1);
            }
        }

        return {&keyFrames[prevKeyFrame], &keyFrames[nextKeyFrame]};
    }

    float Animation::getDuration() 
    { 
        return duration; 
    }

    KeyFrame KeyFrame::lerp(KeyFrame & from, KeyFrame & to, float progress)
    {
        KeyFrame keyFrame;
        keyFrame.timeStamp = from.timeStamp + progress * (to.timeStamp - from.timeStamp);

        keyFrame.jointTransforms.resize(from.jointTransforms.size());
        for (size_t i = 0; i < keyFrame.jointTransforms.size(); i++)
        {
            keyFrame.jointTransforms[i] = JointTransform::lerp(from.jointTransforms[i], to.jointTransforms[i], progress);
        }

        return keyFrame;
    }

    KeyFrame::KeyFrame()
    {
        timeStamp = 0;
    }

    KeyFrame::KeyFrame(Hybris::KeyFrame & keyFrame)
        : timeStamp(keyFrame.timeStamp)
    {
        jointTransforms.resize(keyFrame.jointTransforms.size);
        for (size_t i = 0; i < keyFrame.jointTransforms.size; i++)
        {
            jointTransforms[keyFrame.jointTransforms[i].jointId] = keyFrame.jointTransforms[i];
        }
    }


    JointTransform JointTransform::lerp(JointTransform & from, JointTransform & to, float progress)
    {
        if (from == to) return from;

        JointTransform jointTransform;

        jointTransform.translation = DirectX::SimpleMath::Vector3::Lerp(from.translation, to.translation, progress);
        jointTransform.rotation = DirectX::SimpleMath::Quaternion::Lerp(from.rotation, to.rotation, progress);
        jointTransform.scale = DirectX::SimpleMath::Vector3::Lerp(from.scale, to.scale, progress);

        return jointTransform;
    }

    JointTransform::JointTransform() 
    {
    }

    JointTransform::JointTransform(Hybris::JointTransform & jointTransform)
    {
        translation = DirectX::SimpleMath::Vector3(
            jointTransform.translation[0],
            jointTransform.translation[1],
            jointTransform.translation[2]
        );

        rotation = DirectX::SimpleMath::Quaternion(
            jointTransform.rotation[0],
            jointTransform.rotation[1],
            jointTransform.rotation[2],
            jointTransform.rotation[3]
        );

        scale = DirectX::SimpleMath::Vector3(
            jointTransform.scale[0],
            jointTransform.scale[1],
            jointTransform.scale[2]
        );
    }
    
    DirectX::SimpleMath::Matrix JointTransform::createMatrix()
    {
        DirectX::SimpleMath::Matrix t = DirectX::SimpleMath::Matrix::CreateTranslation(translation);
        DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
        DirectX::SimpleMath::Matrix s = DirectX::SimpleMath::Matrix::CreateScale(scale);

        return DirectX::XMMatrixAffineTransformation(scale, {}, rotation, translation);
        //return t * r;
    }
    bool JointTransform::operator==(JointTransform const & other) const
    {
        if (this == &other) return true;
        return translation == other.translation
            && rotation == other.rotation
            && scale == other.scale;
    }
}