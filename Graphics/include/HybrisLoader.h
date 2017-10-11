#pragma once
#include <Hybris.h>
#include <map>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>

enum class FileID
{
    TestCubeA
};

class Model
{
public:

    Model(Hybris::File & file);

    std::vector<DirectX::SimpleMath::Matrix> getPoseTransforms(const char * animationName, float timeStamp);

private:
    class JointTransform
    {
    public:
        static DirectX::SimpleMath::Matrix lerp(JointTransform & from, JointTransform & to, float progress);
        JointTransform(Hybris::JointTransform & jointTransform);
    private:
        DirectX::SimpleMath::Vector3 translation;
        DirectX::SimpleMath::Quaternion rotation;
        DirectX::SimpleMath::Vector3 scale;
    };
    class KeyFrame
    {
    public:
        static std::vector<DirectX::SimpleMath::Matrix> lerp(KeyFrame & from, KeyFrame & to, float progress);

        KeyFrame(Hybris::KeyFrame & keyFrame);
        const float timeStamp;
    private:
        std::map<size_t, JointTransform> jointTransforms;
    };
    class Animation
    {
    public:
        Animation(Hybris::Animation & animation);
        std::pair<KeyFrame*, KeyFrame*> getCurrentAndNextFrame(float timeStamp);
    private:
        std::string name;
        std::vector<KeyFrame> keyFrames;
    };
    class Joint
    {
    public:
        Joint(Hybris::Joint & joint);
        DirectX::SimpleMath::Matrix invBindTransform;
    private:
    };
    class Skeleton
    {
    public:
        Skeleton(Hybris::List<Hybris::Joint> & jointList);

        std::vector<DirectX::SimpleMath::Matrix> evaluateJointTransforms(std::vector<DirectX::SimpleMath::Matrix> & jointTransforms);
    private:
        std::map<size_t, Joint> joints;
    };

    constexpr size_t hash(const char * str) const;

    Skeleton skeleton;
    std::map<size_t, Animation> animations;

    Animation * getAnimation(const char * animationName);
};


class HybrisLoader
{
public:
    HybrisLoader();
    virtual ~HybrisLoader();

    Model * getModel(FileID fileId) { return &models.at(fileId); }
private:
    std::map<FileID, Model> models;
};