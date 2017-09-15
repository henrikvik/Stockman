struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};


float4 positions[] = { };

float4 VS(uint id : SV_VertexID) : SV_Position
{
    switch (id)
    {
        case 0:
            return float4(-2, 1, 0, 1);
        case 1:
            return float4(1, -2, 0, 1);
        case 2:
            return float4(1, 1, 0, 1);
        
    }
}

Texture2D albedoSpec : register(t0);
Texture2D normalMat : register(t1);
Texture2D pos : register(t2);
SamplerState sState;

float4 PS(float4 position : SV_Position) : SV_Target0
{
    //return float4(0.2, 1, 1, 1);
    return float4(albedoSpec.Load(int3(position.x, position.y, 0)).xyz, 1);
};