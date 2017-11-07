
struct StaticInfo
{
    float4x4 world;
    float4x4 worldInvT;
    float freeze;
    float burn;
};

StructuredBuffer<StaticInfo> instanceBuffer : register(t10);
