struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : UV;
};

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;

    vsout.uv = float2((id << 1) & 2, id & 2);
    vsout.position = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);

    return vsout;
}

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


float3 CalculateNormal(const float4 edges, float3 center, float3 left, float3 right, float3 top, float3 bottom)
{
    // x = left
    // y = right
    // z = top
    // w = bottom

    float4 acceptedNormals = float4(edges.x * edges.z, edges.z * edges.y, edges.y * edges.w, edges.w * edges.x);

    left = normalize(left - center);
    right = normalize(right - center);
    top = normalize(top - center);
    bottom = normalize(bottom - center);

    float3 pixelNormal = float3(0, 0, -0.0005);
    pixelNormal += (acceptedNormals.x) * cross(left, top);
    pixelNormal += (acceptedNormals.y) * cross(top, right);
    pixelNormal += (acceptedNormals.z) * cross(right, bottom);
    pixelNormal += (acceptedNormals.w) * cross(bottom, left);
    pixelNormal = normalize(pixelNormal);

    return pixelNormal;
}

float4 CalculateEdges(const float center, const float left, const float right, const float top, const float bottom)
{
    float4 edges = float4(left, right, top, bottom) - center;
    float4 edgesSlope= edges + edges.yxwz;
    edges = min(abs(edges), abs(edgesSlope));

    return saturate((1.3 - edges / (center * 0.040)));
}

float ScreenSpaceToViewSpaceDepth(float depth)
{
    float linearizeMul = DepthUnpack.x;
    float linearizeAdd = DepthUnpack.y;

    return linearizeMul / (linearizeAdd - depth);
}

float2 ScreenSpaceToClipSpacePositionXY(float2 screen)
{
    return screen * Viewport2X.xy - float2(1.f, 1.f);
}

float2 ScreenSpaceToViewSpacePosition(float2 screen, float depth)
{
    return float3(CameraHalfFOV.xy * depth * ScreenSpaceToClipSpacePositionXY(screen), depth);
}

float3 NDCToViewSpace(float2 ndc, float depth)
{
    return float3((NDCToViewMul * ndc.xy + NDCToViewAdd) * depth, depth);
}

Texture2D Depth : register(t0);
SamplerState PointClamp : register(s0);

struct Targets {
    float DepthInterleaveTL : SV_Target0;
    float DepthInterleaveBR : SV_Target1;
};

RWTexture2D<unorm float4> Normals : register(u2);

Targets PS(VSOutput input)
{
    Targets targets;

    int2 base = ((int2)input.position.xy) * 2;
    float2 tl = (input.position.xy - 0.25) * Viewport2X;

    int3 coord = int3(base, 0);

    float z0 = ScreenSpaceToViewSpaceDepth(Depth.Load(coord, int2(0, 0)).x);
    float z1 = ScreenSpaceToViewSpaceDepth(Depth.Load(coord, int2(1, 0)).x);
    float z2 = ScreenSpaceToViewSpaceDepth(Depth.Load(coord, int2(0, 1)).x);
    float z3 = ScreenSpaceToViewSpaceDepth(Depth.Load(coord, int2(1, 1)).x);

    targets.DepthInterleaveTL = z0;
    targets.DepthInterleaveBR = z3;

    float depthArea[4][4];

    // inner quadrant
    depthArea[1][1] = z0;
    depthArea[2][1] = z1;
    depthArea[1][2] = z2;
    depthArea[2][2] = z3;

    // left edge
    depthArea[0][1] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(-1, 0)).x);
    depthArea[0][2] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(-1, 1)).x);

    // right edge
    depthArea[3][1] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(2, 0)).x);
    depthArea[3][2] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(2, 1)).x);

    // top edge
    depthArea[1][0] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(0, -1)).x);
    depthArea[2][0] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(1, -1)).x);

    // bottom edge
    depthArea[1][3] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(0, 2)).x);
    depthArea[2][3] = ScreenSpaceToViewSpaceDepth(Depth.SampleLevel(PointClamp, tl, .0, int2(1, 2)).x);

    float4 e0 = CalculateEdges(depthArea[1][1], depthArea[0][1], depthArea[2][1], depthArea[1][0], depthArea[1][2]);
    float4 e1 = CalculateEdges(depthArea[2][1], depthArea[2][1], depthArea[3][1], depthArea[2][0], depthArea[2][2]);
    float4 e2 = CalculateEdges(depthArea[1][2], depthArea[1][2], depthArea[2][2], depthArea[1][1], depthArea[1][3]);
    float4 e3 = CalculateEdges(depthArea[2][2], depthArea[1][2], depthArea[3][2], depthArea[2][1], depthArea[2][3]);

    float3 posArea[4][4];

    posArea[1][1] = NDCToViewSpace(tl + Viewport * float2(0., 0.), depthArea[1][1]);
    posArea[2][1] = NDCToViewSpace(tl + Viewport * float2(1., 0.), depthArea[2][1]);
    posArea[1][2] = NDCToViewSpace(tl + Viewport * float2(0., 1.), depthArea[1][2]);
    posArea[2][2] = NDCToViewSpace(tl + Viewport * float2(1., 1.), depthArea[2][2]);

    posArea[0][1] = NDCToViewSpace(tl + Viewport * float2(-1., 0.), depthArea[0][1]);
    posArea[3][2] = NDCToViewSpace(tl + Viewport * float2(2., 1.), depthArea[3][2]);
    posArea[1][0] = NDCToViewSpace(tl + Viewport * float2(0., -1.), depthArea[1][0]);
    posArea[2][3] = NDCToViewSpace(tl + Viewport * float2(1., 2.), depthArea[2][3]);

    float3 n0 = CalculateNormal(e0, posArea[1][1], posArea[0][1], posArea[2][1], posArea[1][0], posArea[1][2]);
    float3 n3 = CalculateNormal(e3, posArea[2][2], posArea[1][2], posArea[3][2], posArea[2][1], posArea[2][3]);

    Normals[coord + int2(0, 0)] = float4(n0 * 0.5 + 0.5, 0.);
    Normals[coord + int2(1, 1)] = float4(n3 * 0.5 + 0.5, 0.);

    return targets;
}