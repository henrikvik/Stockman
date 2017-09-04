#pragma once
#include <Windows.h>
#include <vector>
#include <d3d11.h>

class Camera;
class Mat4;

union AnimationInfo
{

};

struct RenderInfo
{
    bool render;
    int meshId;
    int materialId;
    Mat4 * translation;	
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
    GBuffer gbuffer;

    void createGBuffer();
    void draw(RenderInfo* renderInfo);
};