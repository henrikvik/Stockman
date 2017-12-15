struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : UV;
};

cbuffer SSAOConstants : register(b0)
{
    int2 PerPassFullResCoordOffset;
    float2 NDCToViewMul;
    float2 NDCToViewAdd;
    float2 DepthUnpack;
    float2 CameraHalfFOV;
    float2 HalfViewport;
    float2 Viewport;
    float2 Viewport2X;
    float2 Viewport2X025;
    float2 _Padding;
    float4 PatternRotScaleMatrices[5];
}

SamplerState LinearClamp : register(s2);
Texture2D AO : register(t1);

float PS(VSOutput input) : SV_Target0
{
    float2 halfpixel = HalfViewport * .5f;

    float occlusion = AO.SampleLevel(LinearClamp, input.uv, 0.0).x;

    float a = AO.SampleLevel(LinearClamp, input.uv + float2(-halfpixel.x * 3, -halfpixel.y), 0.0).x;
    float b = AO.SampleLevel(LinearClamp, input.uv + float2( halfpixel.x, -halfpixel.y * 3), 0.0).x;
    float c = AO.SampleLevel(LinearClamp, input.uv + float2(-halfpixel.x,  halfpixel.y * 3), 0.0).x;
    float d = AO.SampleLevel(LinearClamp, input.uv + float2( halfpixel.x * 3, halfpixel.y), 0.0).x;

    return dot(float4(a, b, c, d), float4(0.2, 0.2, 0.2, 0.2)) + occlusion.x * 0.2;
}