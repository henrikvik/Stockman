#include "include/Vertex.hlsli"

struct AnimatedJoints
{
    float4x4 jointTransforms[32];
};

float4x4 calcAnimationMatrix(Vertex vertex, StructuredBuffer<AnimatedJoints> instances, uint instanceId)
{
    float4x4 animationMatrix = float4x4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

    for (int i = 0; i < 4; i++)
    {
        animationMatrix += mul(instances[instanceId].jointTransforms[vertex.jointIds[i]], vertex.jointWeights[i]);
    }

    return animationMatrix;
}
