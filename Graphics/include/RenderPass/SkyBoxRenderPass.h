#pragma once
#include "RenderPass.h"
#include "..\HybrisLoader\HybrisLoader.h"
#include "../Utility/ConstantBuffer.h"
#include "../Utility/Shader.h"
namespace Graphics
{
    //Works kind of, but has to be done before the environment, Markus might fix
    class SkyBoxRenderPass : public RenderPass
    {
    public:
        SkyBoxRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);
        ~SkyBoxRenderPass() {};

        virtual void render() const;
        virtual void update(float deltaTime);

    private:
        HybrisLoader::Mesh * skySphere;
        ConstantBuffer<DirectX::SimpleMath::Matrix> sphereTransformBuffer;
        Shader skyShader;
    };
}