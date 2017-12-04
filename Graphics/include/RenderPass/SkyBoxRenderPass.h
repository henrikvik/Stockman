#pragma once
#include "RenderPass.h"
#include "..\HybrisLoader\HybrisLoader.h"
#include "../Utility/ConstantBuffer.h"
#include "../Utility/Shader.h"
#include "../Utility/ShaderResource.h"
#include "../Lights/Sun.h"

namespace Graphics
{

    struct SkyParams {
        DirectX::SimpleMath::Vector4 base;
        DirectX::SimpleMath::Vector4 top;
        DirectX::SimpleMath::Vector4 angleSize;
        DirectX::SimpleMath::Matrix view;
    };
    //Works kind of, but has to be done before the environment, Markus might fix
    class SkyBoxRenderPass : public RenderPass
    {
    public:
        SkyBoxRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources,
            std::initializer_list<ID3D11Buffer*> buffers,
            ID3D11DepthStencilView * depthStencil,
            Sun *sun);
        virtual ~SkyBoxRenderPass();

        virtual wchar_t* name() const override {
            return L"SkyBoxRenderPass";
        }

        virtual void render() const;
        virtual void update(float deltaTime);

    private:
        HybrisLoader::Mesh * skySphere;
        ConstantBuffer<DirectX::SimpleMath::Matrix> sphereTransformBuffer;

        SkyParams m_Colors;
        ConstantBuffer<SkyParams> m_SkyColors;

        ID3D11BlendState* m_BlendState;
        ID3D11ShaderResourceView *m_MoonTexture;
        Shader m_MoonShader;
        Shader skyShader;
        Sun *m_Sun;
    };
}