#include "Joint.h"

Joint::Joint(UINT id, Matrix invBindTransform)
    : id(id)
    , invBindTransform(invBindTransform)
{
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
