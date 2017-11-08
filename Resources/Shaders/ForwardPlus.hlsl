#include "LightCalcInclude.hlsli"


#define INSTANCE_T_SLOT t10
#include "Instance.hlsli"

#define VERTEX_T_SLOT t11
#include "Vertex.hlsli"


#define USE_GRID_TEXTURE true

struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 worldPos : POS;
    float4 lightPos : LIGHT_POS;
    float3 normal : NORMAL;
    float3 normalView : NORMALVIEW;
    float2 uv : UV;
    float3 biTangent : BITANGENT;
    float3 tangent : TANGENT;
    
    float2 gridUV : GridUV;

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
#define GET_COL(mat, col) float3(mat[0][col],mat[1][col],mat[2][col])
#define GET_ROW(mat, row) float3(mat[row][0],mat[row][1],mat[row][2])
VSOutput VS(uint vertexId : SV_VertexId, uint instanceId : SV_InstanceId) 
{
    Vertex vertex = getVertex(vertexId);
    Instance instance = getInstance(instanceId);

	VSOutput output;


    output.worldPos = mul(instance.world, float4(vertex.position, 1));
    output.pos = mul(ViewProjection, output.worldPos);
    output.freeze = instance.freeze;
    output.burn = instance.burn;

	output.uv = vertex.uv;
    
    output.normal = mul(instance.worldInvT, float4(vertex.normal, 0));
    output.normal = normalize(output.normal);

    output.normalView = normalize(mul(View, float4(output.normal, 0)));

    output.lightPos = output.worldPos + float4(output.normal * 0.15f, 0);
    output.lightPos = mul(lightVP, output.lightPos);

    output.biTangent = normalize(mul(instance.world, float4(vertex.binormal, 0)));
    output.tangent = normalize(mul(instance.world, float4(vertex.tangent, 0)));


    float3 localPosition = mul(instance.world, float4(vertex.position, 0));
    float3x3 tangentMatrix = float3x3(output.tangent, output.biTangent, output.normal);
    float3 worldTangent = mul(tangentMatrix, localPosition);
    output.gridUV = worldTangent.xy / 4;

	return output;
}


[earlydepthstencil]
PSOutput PS(VSOutput input) {
	PSOutput output;


    
//#ifdef USE_GRID_TEXTURE
    input.uv = input.gridUV;    
//#endif

    float3 normal = getNormalMappedNormal(input.tangent, input.biTangent, input.normal, input.uv);
    float shadow = calculateShadowValue(input.lightPos.xyz, 2);
    float3 lighting = calculateDiffuseLight(input.worldPos.xyz, input.lightPos.xyz, input.pos.xyz, input.uv, input.normal, shadow);
    lighting += calculateSpecularity(input.worldPos.xyz, input.lightPos.xyz, input.pos.xyz, input.uv, input.normal, shadow);
    
    lighting = calculateStatusEffect(lighting, input.freeze, input.burn);

    lighting = saturate(lighting);
    lighting += float3(0.2, 0.2, 0.2);
    
    output.backBuffer = float4(lighting, 1); //500~
    output.glowMap = glowMap.Sample(Sampler, input.uv); //300~
    output.normalView = float4(input.normalView.xyz, 1); //300~
    
    

    
    return output;
}