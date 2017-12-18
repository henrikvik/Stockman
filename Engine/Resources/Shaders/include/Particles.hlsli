struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float age : AGE;
    float noisescale : NOISESCALE;
    float noisespeed : NOISESPEED;
    float deform : DEFORM;
};