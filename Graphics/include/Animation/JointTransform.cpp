#include "JointTransform.h"

JointTransform::JointTransform(Vector3 position, Quaternion rotation)
    : position(position)
    , rotation(rotation)
{
}

JointTransform::~JointTransform()
{
}

Matrix JointTransform::interpolateTo(JointTransform const &other, float progress) const
{
    Matrix r = Matrix::CreateFromQuaternion(Quaternion::Slerp(rotation, other.rotation, progress));
    Matrix p = Matrix::CreateTranslation(Vector3::Lerp(position, other.position, progress));
    return p * r;
}
