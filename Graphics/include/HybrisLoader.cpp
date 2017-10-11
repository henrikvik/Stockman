#include "HybrisLoader.h"
#include <Engine\Constants.h>

HybrisLoader::HybrisLoader()
{
    auto testCubeA = Hybris::readFromFile(ANIMATION_PATH("TestCubeA.hyb"));
    models.insert_or_assign(FileID::TestCubeA, testCubeA);
    testCubeA.release();
}

HybrisLoader::~HybrisLoader()
{

}

constexpr size_t Model::hash(const char * str) const
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

Model::Animation * Model::getAnimation(const char * animationName)
{
    size_t hashv = hash(animationName);
    return &animations.at(hashv);
}

Model::Model(Hybris::File & file)
    : skeleton(file.joints)
{
    for (size_t i = 0; i < file.animations.count; i++)
    {
        animations.insert_or_assign(hash(file.animations[i].name), file.animations[i]);
    }
}

std::vector<DirectX::SimpleMath::Matrix> Model::getPoseTransforms(const char * animationName, float timeStamp)
{
    Animation * animation = getAnimation(animationName);
    std::pair<KeyFrame*, KeyFrame*> keyFrames = animation->getCurrentAndNextFrame(timeStamp);

    float current = keyFrames.first->timeStamp;
    float next = keyFrames.second->timeStamp;
    float progress = (timeStamp - current) / (next - current + 0.001f);

    return skeleton.evaluateJointTransforms(
        KeyFrame::lerp(*keyFrames.first, *keyFrames.second, progress)
    );
}

Model::Skeleton::Skeleton(Hybris::List<Hybris::Joint>& jointList)
{
    for (size_t i = 0; i < jointList.count; i++)
    {
        joints.insert_or_assign(jointList[i].id, jointList[i]);
    }
}

std::vector<DirectX::SimpleMath::Matrix> Model::Skeleton::evaluateJointTransforms(std::vector<DirectX::SimpleMath::Matrix>& jointTransforms)
{
    std::vector<DirectX::SimpleMath::Matrix> transformed(jointTransforms.size());

    for (size_t i = 0, size = jointTransforms.size(); i < size; i++)
    {
        transformed[i] = jointTransforms[i] * joints.at(i).invBindTransform;
    }

    return transformed;
}

Model::Joint::Joint(Hybris::Joint & joint)
{
    for (size_t x = 0, y; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            invBindTransform.m[x][y] = joint.invBindTransform[x][y];
        }
    }
}

Model::Animation::Animation(Hybris::Animation & animation)
{
    name = animation.name;
    for (size_t i = 0; i < animation.keyFrames.count; i++)
    {
        keyFrames.emplace_back(animation.keyFrames[i]);
    }
}

std::pair<Model::KeyFrame*, Model::KeyFrame*> Model::Animation::getCurrentAndNextFrame(float timeStamp)
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

    return { &keyFrames[prevKeyFrame], &keyFrames[nextKeyFrame] };
}

std::vector<DirectX::SimpleMath::Matrix> Model::KeyFrame::lerp(KeyFrame & from, KeyFrame & to, float progress)
{
    std::vector<DirectX::SimpleMath::Matrix> interpolatedTransforms(from.jointTransforms.size());

    for (auto & jointId_jointTransform : from.jointTransforms)
    {
        size_t           jointId        = jointId_jointTransform.first;
        JointTransform & jointTransform = jointId_jointTransform.second;

        interpolatedTransforms[jointId] =
            JointTransform::lerp(
                jointTransform,
                to.jointTransforms.at(jointId),
                progress
            );
    }

    return interpolatedTransforms;
}

Model::KeyFrame::KeyFrame(Hybris::KeyFrame & keyFrame)
    : timeStamp(keyFrame.timeStamp)
{
    for (size_t i = 0; i < keyFrame.jointTransforms.count; i++)
    {
        jointTransforms.insert_or_assign(
            keyFrame.jointTransforms[i].jointId, 
            keyFrame.jointTransforms[i]
        );
    }
}

DirectX::SimpleMath::Matrix Model::JointTransform::lerp(JointTransform & from, JointTransform & to, float progress)
{
    using namespace DirectX::SimpleMath;

    Matrix rotate = Matrix::CreateFromQuaternion(Quaternion::Lerp(from.rotation, to.rotation, progress));;
    Matrix scale = Matrix::CreateScale(Vector3::Lerp(from.scale, to.scale, progress));
    Matrix translate = Matrix::CreateTranslation(Vector3::Lerp(from.translation, to.translation, progress));

    return translate * scale * rotate;
}

Model::JointTransform::JointTransform(Hybris::JointTransform & jointTransform)
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
