/*
*  Author: fkaa
*
*/
#include "LightCalcInclude.hlsli"



//Used by VS
struct InstanceData
{
    float4x4 world;
    float4x4 invWorldT;
    float freeze;
    float burn;
};

StructuredBuffer<InstanceData> instanceData : register(t20);
cbuffer InstanceOffsetBuffer : register(b4)
{
    uint instanceOffset;
}

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float2 biTangent : BITANGENT;
	float2 tangent : TANGENT;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float4 worldPos : POS;
    float4 lightPos : LIGHT_POS;
	float3 normal : NORMAL;
    float3 normalView : NORMALVIEW;
	float2 uv : UV;
    float3 biTangent : BITANGENT;
    float3 tangent : TANGENT;

    //Change this to struct later
    float freeze : FREEZE;
    float burn : BURN;
};

struct PSOutput
{
    float4 backBuffer : SV_Target0;
    float4 glowMap : SV_Target1;
    float4 normalView : SV_Target2;
};

VSOutput VS(VSInput input, uint instanceId : SV_InstanceId) {
	VSOutput output;

    float4x4 world = instanceData[instanceOffset + instanceId].world;
    float4x4 invWorldT = instanceData[instanceOffset + instanceId].invWorldT;
    output.freeze = instanceData[instanceOffset + instanceId].freeze;
    output.burn = instanceData[instanceOffset + instanceId].burn;

	output.worldPos = mul(world, float4(input.position, 1));
    output.pos = mul(ViewProjection, output.worldPos);

	output.uv = input.uv;

    //Temporary fix until normals are RH
    input.normal.z *= -1;

    
    output.normal = mul(invWorldT, float4(input.normal, 0));
    output.normal = normalize(output.normal);

    output.normalView = normalize(mul(View, float4(output.normal, 0)));

    output.lightPos = output.worldPos + float4(output.normal * 0.15f, 0);
    output.lightPos = mul(lightVP, output.lightPos);

    output.tangent.xy = input.tangent.xy;
    output.biTangent.xy = input.biTangent.xy;


    output.tangent.z = sqrt(1 - pow(output.tangent.x, 2) + pow(output.tangent.y, 2));
    output.biTangent.z = sqrt(1 - pow(output.biTangent.x, 2) + pow(output.biTangent.y, 2));


    output.biTangent = normalize(mul(world, output.biTangent));
    output.tangent = normalize(mul(world, output.tangent));

	return output;
}

[earlydepthstencil]
PSOutput PS(VSOutput input) {
	PSOutput output;

    float3 normal = getNormalMappedNormal(input.tangent, input.biTangent, input.normal, input.uv);
    float shadow = calculateShadowValue(input.lightPos.xyz, 2);
    float3 lighting = calculateDiffuseLight(input.worldPos.xyz, input.lightPos.xyz, input.pos.xyz, input.uv, input.normal, shadow);
    lighting += calculateSpecularity(input.worldPos.xyz, input.lightPos.xyz, input.pos.xyz, input.uv, input.normal, shadow);
    
    lighting = calculateStatusEffect(lighting, input.freeze, input.burn);

    lighting = saturate(lighting);
    
    
    output.backBuffer = float4(lighting, 1); //500~
    output.glowMap = glowMap.Sample(Sampler, input.uv); //300~
    output.normalView = float4(input.normalView.xyz, 1); //300~
    
    return output;
}