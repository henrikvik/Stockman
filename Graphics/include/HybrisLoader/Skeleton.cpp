#include "Skeleton.h"
#include <unordered_map>

namespace HybrisLoader
{
    Skeleton::Skeleton(Hybris::Skeleton & skeleton)
    {
        joints.resize(skeleton.joints.size);
        for (size_t i = 0; i < skeleton.joints.size; i++)
        {
            joints[skeleton.joints[i].id] = skeleton.joints[i];
        }

        for (size_t i = 0; i < skeleton.animations.size; i++)
        {
            animations.insert_or_assign(hash(skeleton.animations[i].name), skeleton.animations[i]);
        }
    }

    std::vector<DirectX::SimpleMath::Matrix> Skeleton::evaluateJointTransforms(KeyFrame & keyFrame)
    {
        std::vector<DirectX::SimpleMath::Matrix> transformed(joints.size());

        for (size_t i = 0; i < transformed.size(); i++)
        {
            transformed[i] =
                keyFrame.jointTransforms.at(i).createMatrix() 
                * 
                joints.at(i).invBindTransform  
                //SM::Matrix()
                ;
        }

        return transformed;
    }

    std::vector<DirectX::SimpleMath::Matrix> Skeleton::evalAnimation(const char * animationName, float timeStamp)
    {
        Animation * animation = getAnimation(animationName);
        std::pair<KeyFrame*, KeyFrame*> current_next = animation->getCurrentAndNextFrame(timeStamp);

        float current = current_next.first->timeStamp;
        float next = current_next.second->timeStamp;
        float progress = (timeStamp - current) / (next - current + FLT_EPSILON);

        KeyFrame keyFrame = KeyFrame::lerp(*current_next.first, *current_next.second, progress);

        return evaluateJointTransforms(keyFrame);
    }

    Joint::Joint()
    {
    }

    Joint::Joint(Hybris::Joint & joint)
    {
        for (size_t x = 0, y; x < 4; x++)
        {
            for (y = 0; y < 4; y++)
            {
                invBindTransform.m[x][y] = joint.invBindTransform[x][y];
            }
        }
    }


    constexpr size_t Skeleton::hash(const char * str) const
    {
        size_t value = 0;

        size_t i = 0;
        while (*(str + i))
        {
            value += (size_t)*(str + i) * pow(i + 1, i + 1);
            i++;
        }

        return value;
    }
    
    Animation * Skeleton::getAnimation(const char * animationName)
    {
        return &animations.at(hash(animationName));
    }
}