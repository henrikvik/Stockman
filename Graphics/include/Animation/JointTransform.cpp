#include "JointTransform.h"

JointTransform::JointTransform(Vector3 position, Quaternion rotation)
    : position(position)
    , rotation(rotation)
{
}

JointTransform::~JointTransform()
{
}

Matrix JointTransform::interpolateTo(JointTransform const & jointTransform, float progress) const
{
    return Matrix::CreateTranslation(
        Vector3::Lerp(position, jointTransform.position, progress)
    );
}
