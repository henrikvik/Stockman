#pragma once
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include "../Camera.h"
#include <unordered_map>

union AnimationInfo
{

};

struct RenderInfo
{
    bool render;
    int meshId;
    int materialId;
    DirectX::SimpleMath::Matrix translation;	
};


struct GBuffer
{
    ID3D11RenderTargetView * diffuseSpec;   // Dr Dg Db S
    ID3D11RenderTargetView * normalMat;     // Nx Ny Nz Mid
    ID3D11RenderTargetView * position;      // Px Py Pz Py
    ID3D11DepthStencilView * depth;
    ID3D11ShaderResourceView * diffuseSpecView;
    ID3D11ShaderResourceView * normalMatView;
    ID3D11ShaderResourceView * positionView;
    ID3D11ShaderResourceView * depthView;
};

struct InstanceData
{
    DirectX::SimpleMath::Matrix translation;
};


class Renderer
{
public:
    Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
    void render(Camera * camera);
    void qeueuRender(RenderInfo * renderInfo);
private:
    ID3D11Device * device;
    ID3D11DeviceContext * deviceContext;

    std::vector<RenderInfo*> renderQueue;
    typedef  std::unordered_map<int, std::vector<InstanceData>> InstanceQueue_t;
    InstanceQueue_t instanceQueue;
    GBuffer gbuffer;

    ID3D11Buffer *instanceBuffer;

    void createGBuffer();
    void cull();
    void draw();
};