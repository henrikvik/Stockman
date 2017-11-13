#include "SkyBoxRenderPass.h"
#include "../Device.h"
#include "../MainCamera.h"
#include "../CommonState.h"

namespace Graphics
{
    SkyBoxRenderPass::SkyBoxRenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil) :
        RenderPass(targets, resources, buffers, depthStencil),
        skyShader(Resources::Shaders::SkyShader),
        sphereTransformBuffer(Global::device)
    {
        this->skySphere = &HybrisLoader::HybrisLoader::get().getModel(Resources::Models::SkySphere)->getMesh();

    }

    void SkyBoxRenderPass::render() const
    {
        PROFILE_BEGIN("Sky Box");
        Global::context->RSSetState(Global::cStates->CullCounterClockwise());

        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(skyShader, nullptr, 0);
        Global::context->PSSetShader(skyShader, nullptr, 0);

        Global::context->VSSetShaderResources(4, 1, skySphere->getVertexBuffer());

        Global::context->VSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());
        Global::context->VSSetConstantBuffers(4, 1, sphereTransformBuffer);

        Global::context->PSSetConstantBuffers(1, 1, &buffers[0]);
        Global::context->OMSetRenderTargets(1, &targets[0], depthStencil);

        Global::context->Draw(skySphere->getVertexCount(), 0);

        Global::context->RSSetState(Global::cStates->CullClockwise());
        Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
        PROFILE_END();
    }

    void SkyBoxRenderPass::update(float deltaTime)
    {
        DirectX::SimpleMath::Matrix temp = DirectX::SimpleMath::Matrix::CreateTranslation(Global::mainCamera->getPos());

        sphereTransformBuffer.write(Global::context, &temp, sizeof(temp));
    }
}
