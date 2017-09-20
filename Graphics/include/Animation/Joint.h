#pragma once
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class Joint
{
public:
    Joint(UINT id, Matrix invBindTransform);
    const UINT id;


    inline void addChild(Joint & joint) { children.push_back(joint); }
    inline void setAnimatedTransform(Matrix & transform) { animatedTransform = transform; }
    inline Matrix const& getAnimatedTransform() const { return animatedTransform; }
    inline Matrix const& getInvBindTransform() const { return invBindTransform; }

    //void calcInvBindTransform(Matrix & parentBindTransform);
private:
    std::vector<Joint> children;

    Matrix invBindTransform;
    Matrix animatedTransform;

    //Matrix localBindTransform;
};