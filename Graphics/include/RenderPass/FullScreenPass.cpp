#include "FullScreenPass.h"

Graphics::FullScreenPass::FullScreenPass(
    std::initializer_list<ID3D11RenderTargetView*> targets, 
    std::initializer_list<ID3D11ShaderResourceView*> resources,
    std::initializer_list<ID3D11Buffer*> buffers,
    ID3D11DepthStencilView * depthStencil):
    RenderPass(targets, resources, buffers, depthStencil),
    fullScreenShader(Resources::Shaders::FullscreenQuad)
{

}

void Graphics::FullScreenPass::render() const
{
    Global::context->IASetInputLayout(nullptr);
    Global::context->VSSetShader(fullScreenShader, nullptr, 0);
    Global::context->PSSetShader(fullScreenShader, nullptr, 0);

    auto sampler = Global::cStates->LinearWrap();
    Global::context->PSSetSamplers(0, 1, &sampler);

    //stupid first case
    Global::context->PSSetShaderResources(0, 1, &resources[0]);
    Global::context->OMSetRenderTargets(1, &targets[0], nullptr);

    Global::context->Draw(3, 0);

    Global::context->OMSetRenderTargets(0, Global::nulls, nullptr);
    Global::context->PSSetShaderResources(0, 1, Global::nulls);
}
