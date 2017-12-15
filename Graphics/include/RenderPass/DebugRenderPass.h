#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/ConstantBuffer.h"

namespace Graphics
{

class DebugRenderPass : public RenderPass
{
public:
    DebugRenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources = {},
        std::initializer_list<ID3D11Buffer*> buffers = {},
        ID3D11DepthStencilView * depthStencil = nullptr
    );
    virtual ~DebugRenderPass();

    virtual wchar_t* name() const override {
        return L"DebugRenderPass";
    }

    // Inherited via RenderPass
    virtual void render() const override;
    virtual void update(float deltaTime) override;

private:
    Shader debugShader;

    struct DebugVertex
    {
        DirectX::SimpleMath::Vector4 position;
        DirectX::SimpleMath::Color color;
    };

    ConstantBuffer<UINT> offsetBuffer;
    StructuredBuffer<DebugVertex> vertexBuffer;
};

}