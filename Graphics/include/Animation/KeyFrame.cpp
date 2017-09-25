#include "KeyFrame.h"

KeyFrame::KeyFrame(float timeStamp, std::initializer_list<JointTransform> jointTransforms)
    : timeStamp(timeStamp)
    , jointTransforms(jointTransforms)
{
}

KeyFrame::~KeyFrame()
{
}

std::vector<Matrix> KeyFrame::interpolateTo(KeyFrame const & keyFrame, float progress) const
{
    std::vector<Matrix> interpolatedTransforms;
    interpolatedTransforms.resize(jointTransforms.size());

    for (int i = 0; i < jointTransforms.size(); i++)
    {
        interpolatedTransforms[i] = 
            jointTransforms[i].interpolateTo(keyFrame.jointTransforms[i], progress);
    }

    return interpolatedTransforms;
}
