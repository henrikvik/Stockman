#include "ShadowRenderPass.h"
#include "../Device.h"


ShadowRenderPass::ShadowRenderPass(ID3D11DepthStencilView * shadowMap, D3D11_VIEWPORT viewport, ID3D11Buffer * lightMatrixes)
{
}

void ShadowRenderPass::update(float deltaTime)
{
    sun.update();
}

void ShadowRenderPass::render()
{
    Globals::context->RSSetViewports(1, &sun.getViewPort());
    Globals::context->IASetInputLayout(nullptr);
    Globals::context->VSSetShader(*shader, nullptr, 0);
    Globals::context->PSSetShader(nullptr, nullptr, 0);
    Globals::context->OMSetRenderTargets(0, nullptr, shadowDepthStencil);
    Globals::context->VSSetConstantBuffers(0, 1, *sun.getMatrixBuffer());

    drawinstanced<något_typ>
        clear med
}
