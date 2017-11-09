struct Camera
{
    float4x4 viewProjection : ViewProjection;
    float4x4 invProjection : InvProjection;
    float4x4 view : View;
    float4 position : Position;
};
