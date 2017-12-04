struct Camera
{
    float4x4 viewProjection : ViewProjection;
    float4x4 invProjection : InvProjection;
    float4x4 view : View;
    float4x4 shadowProj : Proj;
    float4 position : Position;
    float4 forward : Forward;
};
