#include "Joint.h"

Joint::Joint(UINT id, Matrix invBindTransform)
    : id(id)
    , invBindTransform(invBindTransform)
{
}

void Joint::getJointTransforms(std::vector<Matrix> const & animationTransforms, std::vector<Matrix>& jointTransforms)
{
    jointTransforms[id] = animationTransforms[id] * invBindTransform;

    for (Joint & child : children)
    {
        child.getJointTransforms(animationTransforms, jointTransforms);
    }
}

//void Joint::calcInvBindTransform(Matrix & parentBindTransform)
//{
//    Matrix bindTransform = parentBindTransform * localBindTransform;
//    bindTransform.Invert(invBindTransform);
//
//    for (Joint * child : children)
//    {
//        child->calcInvBindTransform(bindTransform);
//    }
//}
