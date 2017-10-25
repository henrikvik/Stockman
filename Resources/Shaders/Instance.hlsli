#ifdef INSTANCE_T_SLOT
struct Instance
{
    float4x4 world;
    float4x4 worldInvT;
};

StructuredBuffer<Instance> INSTANCE_DATA : register(INSTANCE_T_SLOT);

Instance getInstance(uint instanceId)
{
    return INSTANCE_DATA[instanceId];
}
#else
#error "INSTANCE_T_SLOT not defined, nowhere to bind INSTANCE_DATA"
#endif