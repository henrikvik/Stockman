#include "Vertex.hlsli"
#include "InstanceStatic.hlsli"

struct InstanceAnimated : InstanceStatic
{
    float4x4 jointTransforms[20];
};

float4x4 calcAnimationMatrix(Vertex vertex, InstanceAnimated instance)
{
    float4x4 animationMatrix;

    for (int i = 0; i < 4; i++)
    {
        animationMatrix += mul(instance.jointTransforms[vertex.jointIds[i]], vertex.jointWeights[i]);
    }

    return animationMatrix;
}
