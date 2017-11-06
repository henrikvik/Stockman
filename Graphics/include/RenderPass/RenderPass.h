#pragma once
#include <initializer_list>
#include <vector>
#include <d3d11.h>

class RenderPass
{
public:
    RenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources = {},
        std::initializer_list<ID3D11Buffer*> buffers = {},
        ID3D11DepthStencilView * depthStencil = nullptr
    );
    virtual ~RenderPass() {};

    virtual void render() const = 0;
    virtual void update(float deltaTime) = 0;

    std::vector<ID3D11RenderTargetView*> targets;
    std::vector<ID3D11ShaderResourceView*> resources;
    std::vector<ID3D11Buffer*> buffers;
    ID3D11DepthStencilView * depthStencil;
};