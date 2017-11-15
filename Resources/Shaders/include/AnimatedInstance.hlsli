#include "include/Vertex.hlsli"
#include "include/StaticInstance.hlsli"

struct AnimatedInstance : StaticInstance
{
    float4x4 jointTransforms[20];
};

float4x4 calcAnimationMatrix(Vertex vertex, AnimatedInstance instance)
{
    float4x4 animationMatrix = float4x4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

    for (int i = 0; i < 4; i++)
    {
        animationMatrix += mul(instance.jointTransforms[vertex.jointIds[i]], vertex.jointWeights[i]);
    }

    return animationMatrix;
}
