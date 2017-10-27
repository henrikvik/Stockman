#include "LightCalcInclude.hlsli"

#define VERTEX_T_SLOT t4
#include "Vertex.hlsli"

#define INSTANCE_T_SLOT t5
#include "Instance.hlsli"


struct VSOutput {
	float4 pos : SV_POSITION;
	float4 worldPos : POS;
    float4 lightPos : LIGHT_POS;
	float3 normal : NORMAL;
    float3 normalView : NORMALVIEW;
	float2 uv : UV;
    float3 biTangent : BITANGENT;
    float3 tangent : TANGENT;
};

struct PSOutput
{
    float4 backBuffer : SV_Target0;
    float4 glowMap : SV_Target1;
    float4 normalView : SV_Target2;
	float4 worldPosMap : SV_Target3;
};

VSOutput VS(uint vertexId : SV_VertexId, uint instanceId : SV_InstanceId) 
{
    Vertex vertex = getVertex(vertexId);
    Instance instance = getInstance(instanceId);

	VSOutput output;

    output.worldPos = mul(instance.world, float4(vertex.position, 1));
    output.pos = mul(ViewProjection, output.worldPos);

	output.uv = vertex.uv;

    //Temporary fix until normals are RH
    //input.normal.z *= -1;

    
    output.normal = mul(instance.worldInvT, float4(vertex.normal, 0));
    output.normal = normalize(output.normal);

    output.normalView = normalize(mul(View, float4(output.normal, 0)));

    output.lightPos = output.worldPos + float4(output.normal * 0.15f, 0);
    output.lightPos = mul(lightVP, output.lightPos);

    //output.tangent.xy = input.tangent.xy;
    //output.biTangent.xy = input.biTangent.xy;


    //output.tangent.z = sqrt(1 - pow(output.tangent.x, 2) + pow(output.tangent.y, 2));
    //output.biTangent.z = sqrt(1 - pow(output.biTangent.x, 2) + pow(output.biTangent.y, 2));


    output.biTangent = normalize(mul(instance.world, float4(vertex.binormal, 0)));
    output.tangent = normalize(mul(instance.world, float4(vertex.tangent, 0)));

	return output;
}

[earlydepthstencil]
PSOutput PS(VSOutput input) {
	PSOutput output;

    float3 normal = getNormalMappedNormal(input.tangent, input.biTangent, input.normal, input.uv);
    float shadow = calculateShadowValue(input.lightPos.xyz, 2);
    float3 lighting = calculateDiffuseLight(input.worldPos.xyz, input.lightPos.xyz, input.pos.xyz, input.uv, normal, shadow);
    lighting += calculateSpecularity(input.worldPos.xyz, input.lightPos.xyz, input.pos.xyz, input.uv, normal, shadow);
    
    lighting = saturate(lighting);
    
    
    output.backBuffer = diffuseMap.Sample(Sampler, input.uv); //float4(lighting, 1);
    //output.glowMap = glowMap.Sample(Sampler, input.uv);
    //output.normalView = float4(input.normalView.xyz, 1);
	//output.worldPosMap = input.worldPos;

    
    
    return output;
}