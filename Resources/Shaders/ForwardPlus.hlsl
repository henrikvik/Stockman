
#include "include/Camera.hlsli"
#include "include/LightCalc.hlsli"
#include "include/Vertex.hlsli"
#include "include/InstanceStatic.hlsli"

#define USE_GRID_TEXTURE

cbuffer cb0 : register(b0) { Camera camera; };
cbuffer cb1 : register(b1) { DirectionalLight globalLight; };

SamplerState                     linearClamp     : register(s0);
SamplerState                     linearWrap      : register(s1);
SamplerComparisonState           comparison      : register(s2);

StructuredBuffer<uint>           lightIndexList  : register(t0);
Texture2D<uint2>                 lightGrid       : register(t1);
StructuredBuffer<Light>          lights          : register(t2);
Texture2D                        shadowMap       : register(t3);


#ifdef USE_GRID_TEXTURE
Texture2D                        gridTexture     : register(t9);
#endif
StructuredBuffer<InstanceStatic> instanceBuffer  : register(t10);
StructuredBuffer<Vertex>         vertexBuffer    : register(t11);
Texture2D                        diffuseTexture  : register(t12);
Texture2D                        normalTexture   : register(t13);
Texture2D                        specularTexture : register(t14);
Texture2D                        glowTexture     : register(t15);


struct Fragment
{
    float4 ndc        : SV_Position;
    float4 position   : Position;

    float3 normal     : Normal;
    float4 normalView : NormalView;
    float3 binormal   : Binormal;
    float3 tangent    : Tangent;
    
    float2 uv         : UV;
#ifdef USE_GRID_TEXTURE
    float2 gridUV     : GridUV;
#endif
};

struct Targets
{
    float4 color      : SV_Target0;
    float4 glow       : SV_Target1;
    float4 normalView : SV_Target2;
};

Fragment VS(uint vertexId : SV_VertexId, uint instanceId : SV_InstanceId) 
{
    Vertex vertex = vertexBuffer[vertexId];
    InstanceStatic instance = instanceBuffer[instanceId];
	Fragment fragment;

    fragment.position   = mul(instance.world, float4(vertex.position, 1));
    fragment.ndc        = mul(camera.viewProjection, fragment.position);
    
    fragment.normal     = normalize(mul(instance.world, float4(vertex.normal, 0))).xyz;
    fragment.binormal   = normalize(mul(instance.world, float4(vertex.binormal, 0))).xyz;
    fragment.tangent    = normalize(mul(instance.world, float4(vertex.tangent, 0))).xyz;
    fragment.uv         = vertex.uv;

    fragment.normalView = normalize(mul(camera.view,    float4(fragment.normal, 0)));

#ifdef USE_GRID_TEXTURE
    float3   localPosition = mul(instance.world, float4(vertex.position, 0)).xyz;
    float3x3 tangentMatrix = float3x3(fragment.tangent, fragment.binormal, fragment.normal);
    float3   worldTangent  = mul(tangentMatrix, localPosition);
    fragment.gridUV = worldTangent.xy / 4;
#endif

	return fragment;
}


[earlydepthstencil]
Targets PS(Fragment fragment) 
{
    Targets targets;
    
    float3 normal = calcNormal(normalTexture.Sample(linearClamp, fragment.uv).xyz, fragment.normal, fragment.binormal, fragment.tangent);
    float shadowFactor = calcShadowFactor(comparison, shadowMap, globalLight, 2);

    float3 lightSum = calcDiffuse(globalLight, fragment.position, normal);

    lightSum += float3(0.2, 0.2, 0.2); // TODO Move ambient to global light

    lightSum += calcAllLights(lightIndexList, lightGrid, lights, fragment.ndc, fragment.position, normal);

    float3 color = float3(0, 0, 0);

    #ifdef USE_GRID_TEXTURE
    color += gridTexture.Sample(linearWrap, fragment.gridUV).xyz;
    #else
    color += diffuseTexture.Sample(linearClamp, fragment.uv).xyz;
    #endif

    targets.color = float4(lightSum * color, 1); //500~
    targets.glow = glowTexture.Sample(linearClamp, fragment.uv); //300~
    targets.normalView = fragment.normalView; //300~

    return targets;
}