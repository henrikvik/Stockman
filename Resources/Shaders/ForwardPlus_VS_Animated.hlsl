#include "include/Vertex.hlsli"
#include "include/AnimatedInstance.hlsli"
#include "include/Fragment.hlsli"
#include "include/Camera.hlsli"

#define USE_GRID_TEXTURE

cbuffer cb0 : register(b0) { Camera camera; };

cbuffer cb10 : register(b10) { uint instanceOffset; };
StructuredBuffer<AnimatedInstance> instanceBuffer : register(t10);
StructuredBuffer<Vertex>           vertexBuffer   : register(t11);

Fragment VS(uint vertexId : SV_VertexId, uint instanceId : SV_InstanceId) 
{
    Vertex vertex = vertexBuffer[vertexId];
    AnimatedInstance instance = instanceBuffer[instanceId + instanceOffset];
	Fragment fragment;

    instance.world = mul(instance.world, calcAnimationMatrix(vertex, instance));
    fragment.position    = mul(instance.world, float4(vertex.position, 1));
    fragment.ndcPosition = mul(camera.viewProjection, fragment.position);
    
    fragment.normal     = normalize(mul(instance.world, float4(vertex.normal, 0))).xyz;
    fragment.binormal   = normalize(mul(instance.world, float4(vertex.binormal, 0))).xyz;
    fragment.tangent    = normalize(mul(instance.world, float4(vertex.tangent, 0))).xyz;
    fragment.viewNormal = normalize(mul(camera.view,    float4(fragment.normal, 0)));

    fragment.uv         = vertex.uv;

#ifdef USE_GRID_TEXTURE
    float3   localPosition = mul(instance.world, float4(vertex.position, 0)).xyz;
    float3x3 tangentMatrix = float3x3(fragment.tangent, fragment.binormal, fragment.normal);
    float3   worldTangent  = mul(tangentMatrix, localPosition);
    fragment.gridUV = worldTangent.xy / 4;
#endif

	return fragment;
}
