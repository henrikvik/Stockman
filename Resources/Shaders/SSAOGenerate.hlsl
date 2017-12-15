struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : UV;
};

static const float4 SamplePattern[32] =
{
    0.78488064,  0.56661671,  1.500000, -0.126083,     0.26022232, -0.29575172,  1.500000, -1.064030,     0.10459357,  0.08372527,  1.110000, -2.730563,    -0.68286800,  0.04963045,  1.090000, -0.498827,
    -0.13570161, -0.64190155,  1.250000, -0.532765,    -0.26193795, -0.08205118,  0.670000, -1.783245,    -0.61177456,  0.66664219,  0.710000, -0.044234,     0.43675563,  0.25119025,  0.610000, -1.167283,
    0.07884444,  0.86618668,  0.640000, -0.459002,    -0.12790935, -0.29869005,  0.600000, -1.729424,    -0.04031125,  0.02413622,  0.600000, -4.792042,     0.16201244, -0.52851415,  0.790000, -1.067055,
    -0.70991218,  0.47301072,  0.640000, -0.335236,     0.03277707, -0.22349690,  0.600000, -1.982384,     0.68921727,  0.36800742,  0.630000, -0.266718,     0.29251814,  0.37775412,  0.610000, -1.422520,
    -0.12224089,  0.96582592,  0.600000, -0.426142,     0.11071457, -0.16131058,  0.600000, -2.165947,     0.46562141, -0.59747696,  0.600000, -0.189760,    -0.51548797,  0.11804193,  0.600000, -1.246800,
    0.89141309, -0.42090443,  0.600000,  0.028192,    -0.32402530, -0.01591529,  0.600000, -1.543018,     0.60771245,  0.41635221,  0.600000, -0.605411,     0.02379565, -0.08239821,  0.600000, -3.809046,
    0.48951152, -0.23657045,  0.600000, -1.189011,    -0.17611565, -0.81696892,  0.600000, -0.513724,    -0.33930185, -0.20732205,  0.600000, -1.698047,    -0.91974425,  0.05403209,  0.600000,  0.062246,
    -0.15064627, -0.14949332,  0.600000, -1.896062,     0.53180975, -0.35210401,  0.600000, -0.758838,     0.41487166,  0.81442589,  0.600000, -0.505648,    -0.24106961, -0.32721516,  0.600000, -1.665244
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

Texture2D Normal : register(t0);
Texture2D Depth : register(t1);

SamplerState PointClamp : register(s0);
SamplerState PointMirror : register(s1);

#define EffectHorizonAngleThreshold 0.2 // 0.0 .. 0.2
#define EffectRadius 1.2 // 0.0 ..
#define EffectSamplingRadiusNearLimitRec (1 / (EffectRadius * 1.2 * 1.5)) // 0.0

#define FadeTo 300.f // 0.0 ..
#define FadeFrom 50.f // 0.0 ..

#define EffectFadeOutMul (-1.f / (FadeTo - FadeFrom))
#define EffectFadeOutAdd (FadeFrom / (FadeTo - FadeFrom) + 1.f)

#define EffectShadowPow 5.0 // 0.5 .. 5.0
#define EffectShadowClamp 0.8 // 0.0 .. 1.0
#define EffectShadowStrength 3.0 // 0.0 .. 20.0

#define DepthPrecisionOffsetMod 0.9992f

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
    float4 edgesSlope = edges + edges.yxwz;
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

float CalculatePixelObscurance(float3 normal, float3 hitDelta, float falloffCalcMulSq)
{
    float lengthSq = dot(hitDelta, hitDelta);
    float NdotD = dot(normal, hitDelta) / sqrt(lengthSq);

    float falloffMult = max(0.0, lengthSq * falloffCalcMulSq + 1.0);

    return max(0, NdotD - EffectHorizonAngleThreshold) * falloffMult;
}

void SSAOTapInner(inout float obscuranceSum, inout float weightSum, const float2 uv, const float3 center, const float3 negViewspaceDir, float3 pixelNormal, const float falloffCalcMulSq, const float weightMod)
{
    // get depth at sample
    float z = Depth.SampleLevel(PointClamp, uv.xy, 0).x;

    // convert to viewspace
    float3 hitPos = NDCToViewSpace(uv.xy, z).xyz;
    float3 hitDelta = hitPos - center;

    float obscurance = CalculatePixelObscurance(pixelNormal, hitDelta, falloffCalcMulSq);
    float weight = 1.0;

    weight *= weightMod;
    obscuranceSum += obscurance * weight;
    weightSum += weight;
}

void SSAOTap(inout float obscuranceSum, inout float weightSum, const int tapIndex, const float2x2 rotScale, const float3 pixCenterPos, const float3 negViewspaceDir, float3 pixelNormal, const float2 normalizedScreenPos, const float falloffCalcMulSq, float weightMod, float2 normXY, float normXYLength)
{
    float2  sampleOffset;
    float   samplePow2Len;

    // patterns
    {
        float4 newSample = SamplePattern[tapIndex];
        sampleOffset = mul(rotScale, newSample.xy);
        samplePow2Len = newSample.w;                      // precalculated, same as: samplePow2Len = log2( length( newSample.xy ) );
        weightMod *= newSample.z;
    }

    // snap to pixel center (more correct obscurance math, avoids artifacts)
    sampleOffset = round(sampleOffset);

    float2 samplingUV = sampleOffset * Viewport2X + normalizedScreenPos;

    SSAOTapInner(obscuranceSum, weightSum, samplingUV, pixCenterPos, negViewspaceDir, pixelNormal, falloffCalcMulSq, weightMod);

    // for the second tap, just use the mirrored offset
    float2 sampleOffsetMirroredUV = -sampleOffset;

    // snap to pixel center (more correct obscurance math, avoids artifacts)
    float2 samplingMirroredUV = sampleOffsetMirroredUV * Viewport2X + normalizedScreenPos;

    SSAOTapInner(obscuranceSum, weightSum, samplingMirroredUV, pixCenterPos, negViewspaceDir, pixelNormal, falloffCalcMulSq, weightMod);
}

void CalculateRadiusParameters(const float pixCenterLength, const float2 pixelDirRBViewspaceSizeAtCenterZ, out float pixLookupRadiusMod, out float effectRadius, out float falloffCalcMulSq)
{
    effectRadius = EffectRadius;

    // leaving this out for performance reasons: use something similar if radius needs to scale based on distance
    //effectRadius *= pow( pixCenterLength, g_ASSAOConsts.RadiusDistanceScalingFunctionPow);

    // when too close, on-screen sampling disk will grow beyond screen size; limit this to avoid closeup temporal artifacts
    const float tooCloseLimitMod = saturate(pixCenterLength * EffectSamplingRadiusNearLimitRec) * 0.8 + 0.2;

    effectRadius *= tooCloseLimitMod;

    // 0.85 is to reduce the radius to allow for more samples on a slope to still stay within influence
    pixLookupRadiusMod = (0.85 * effectRadius) / pixelDirRBViewspaceSizeAtCenterZ.x;

    // used to calculate falloff (both for AO samples and per-sample weights)
    falloffCalcMulSq = -1.0f / (effectRadius*effectRadius);
}

float GenerateSSAOShadowsInternal(const float2 SVPos)
{
    float outShadowTerm;
    float outWeight;
    float2 SVPosRounded = trunc(SVPos);
    uint2 SVPosui = uint2(SVPosRounded); //same as uint2( SVPos )

    const int numberOfTaps = 3;
    float pixZ, pixLZ, pixTZ, pixRZ, pixBZ;

    float4 valuesUL = Depth.GatherRed(PointMirror, SVPosRounded * HalfViewport);
    float4 valuesBR = Depth.GatherRed(PointMirror, SVPosRounded * HalfViewport, int2(1, 1));

    // get this pixel's viewspace depth
    pixZ = valuesUL.y; //float pixZ = g_ViewspaceDepthSource.SampleLevel( g_PointMirrorSampler, normalizedScreenPos, 0.0 ).x; // * g_ASSAOConsts.MaxViewspaceDepth;

                       // get left right top bottom neighbouring pixels for edge detection (gets compiled out on qualityLevel == 0)
    pixLZ = valuesUL.x;
    pixTZ = valuesUL.z;
    pixRZ = valuesBR.z;
    pixBZ = valuesBR.x;

    float2 normalizedScreenPos = SVPosRounded * Viewport2X + Viewport2X025;
    float3 pixCenterPos = NDCToViewSpace(normalizedScreenPos, pixZ); // g

                                                                     // Load this pixel's viewspace normal
    uint2 fullResCoord = SVPosui * 2 + PerPassFullResCoordOffset.xy * 2;
    float3 pixelNormal = Normal.Load(int3(fullResCoord, 0)).xyz * 2 - 1;

    const float2 pixelDirRBViewspaceSizeAtCenterZ = pixCenterPos.z * NDCToViewMul * Viewport2X;  // optimized approximation of:  float2 pixelDirRBViewspaceSizeAtCenterZ = NDCToViewspace( normalizedScreenPos.xy + g_ASSAOConsts.ViewportPixelSize.xy, pixCenterPos.z ).xy - pixCenterPos.xy;

    float pixLookupRadiusMod;
    float falloffCalcMulSq;

    // calculate effect radius and fit our screen sampling pattern inside it
    float effectViewspaceRadius;
    CalculateRadiusParameters(length(pixCenterPos), pixelDirRBViewspaceSizeAtCenterZ, pixLookupRadiusMod, effectViewspaceRadius, falloffCalcMulSq);

    // calculate samples rotation/scaling
    float2x2 rotScale;
    {

        // load & update pseudo-random rotation matrix
        uint pseudoRandomIndex = uint(SVPosRounded.y * 2 + SVPosRounded.x) % 5;
        float4 rs = PatternRotScaleMatrices[pseudoRandomIndex];
        rotScale = float2x2(rs.x * pixLookupRadiusMod, rs.y * pixLookupRadiusMod, rs.z * pixLookupRadiusMod, rs.w * pixLookupRadiusMod);
    }

    // the main obscurance & sample weight storage
    float obscuranceSum = 0.0;
    float weightSum = 0.0;

    // edge mask for between this and left/right/top/bottom neighbour pixels - not used in quality level 0 so initialize to "no edge" (1 is no edge, 0 is edge)
    float4 edgesLRTB = float4(1.0, 1.0, 1.0, 1.0);

    // Move center pixel slightly towards camera to avoid imprecision artifacts due to using of 16bit depth buffer; a lot smaller offsets needed when using 32bit floats
    pixCenterPos *= DepthPrecisionOffsetMod;

    // Used to tilt the second set of samples so that the disk is effectively rotated by the normal
    // effective at removing one set of artifacts, but too expensive for lower quality settings
    float2 normXY = float2(pixelNormal.x, pixelNormal.y);
    float normXYLength = length(normXY);
    normXY /= float2(normXYLength, -normXYLength);
    normXYLength *= 0.4;

    const float3 negViewspaceDir = -normalize(pixCenterPos);


    for (int i = 0; i < numberOfTaps; i++) {
        SSAOTap(obscuranceSum, weightSum, i, rotScale, pixCenterPos, negViewspaceDir, pixelNormal, normalizedScreenPos, falloffCalcMulSq, 1.0, normXY, normXYLength);
    }

    float obscurance = obscuranceSum / weightSum;
    float fadeOut = saturate(pixCenterPos.z * EffectFadeOutMul + EffectFadeOutAdd);

    obscurance = EffectShadowStrength * obscurance;
    obscurance = min(obscurance, EffectShadowClamp);
    obscurance *= fadeOut;

    float occlusion = 1.0 - obscurance;
    occlusion = pow(saturate(occlusion), EffectShadowPow);

    return occlusion;
}

float PS(VSOutput input) : SV_Target0
{
    return GenerateSSAOShadowsInternal(input.position.xy);
}