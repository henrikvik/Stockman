#include "DepthOfFieldRenderPass.h"
#include "../Device.h"
Graphics::DepthOfFieldRenderPass::DepthOfFieldRenderPass(
    std::initializer_list<ID3D11RenderTargetView*> targets,
    std::initializer_list<ID3D11ShaderResourceView*> resources,
    std::initializer_list<ID3D11Buffer*> buffers,
    ID3D11DepthStencilView * depthStencil) :
    RenderPass(targets, resources, buffers, depthStencil),
    blur1col0(WIN_WIDTH, WIN_HEIGHT),
    blur1col1(WIN_WIDTH, WIN_HEIGHT),
    blur2Final(WIN_WIDTH, WIN_HEIGHT),
    CoCcreation(Resources::Shaders::DoFCreateCoC),
    blur1(Resources::Shaders::DoFBlur1),
    blur2(Resources::Shaders::DoFBlur2),
    glue(Resources::Shaders::DoFGlue),
    cbuffer(Global::device)
{
    firstTime = true;
}

void Graphics::DepthOfFieldRenderPass::update(float deltaTime)
{
    if (firstTime)
    {
        updateCoc();
        firstTime = false;
    }
}

void Graphics::DepthOfFieldRenderPass::render() const
{
    Global::context->OMSetBlendState(nullptr, 0, 0xffffffff);

    static ID3D11SamplerState * samplers[] = {
        Global::cStates->PointClamp(),
        Global::cStates->LinearClamp()
    };

    //calculate CoC pass
    ID3D11ShaderResourceView * srv[] =
    {
        resources[0], //ColorBuffer
        resources[1]  //depthBuffer
    };
    Global::context->PSSetConstantBuffers(0, 1, cbuffer);
    Global::context->VSSetShader(CoCcreation, nullptr, 0);
    Global::context->PSSetShader(CoCcreation, nullptr, 0);

    Global::context->PSSetSamplers(0, 2, samplers);
    Global::context->PSSetShaderResources(0, 2, srv);

    Global::context->OMSetRenderTargets(1, blur2Final, nullptr);
    Global::context->Draw(3, 0);

    Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);

    //blur pass 1
    Global::context->PSSetShaderResources(0, 1, blur2Final);


    Global::context->PSSetShader(blur1, nullptr, 0);


    ID3D11RenderTargetView * rtv[] =
    {
        blur1col0,
        blur1col1
    };
    Global::context->OMSetRenderTargets(2, rtv, nullptr);

    Global::context->Draw(3, 0);


    Global::context->PSSetShaderResources(0, 1, Global::nulls);

    Global::context->OMSetRenderTargets(2, Global::nulls, nullptr);


    //blur pass 2
    Global::context->PSSetShaderResources(0, 1, blur1col0);
    Global::context->PSSetShaderResources(1, 1, blur1col1);

    Global::context->PSSetShader(blur2, nullptr, 0);
    Global::context->OMSetRenderTargets(1, blur2Final, nullptr);

    Global::context->Draw(3, 0);
    Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);

    Global::context->PSSetShaderResources(0, 2, Global::nulls);

    //glue pass
    Global::context->PSSetShaderResources(0, 1, blur2Final);
    Global::context->PSSetShader(glue, nullptr, 0);


    Global::context->OMSetRenderTargets(1, &targets[0], nullptr);
    Global::context->Draw(3, 0);


    Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
    Global::context->PSSetShaderResources(0, 2, Global::nulls);
}

void Graphics::DepthOfFieldRenderPass::updateCoc(float focalLength, float focalPlane, float apature)
{
    //change these if near and far changes
    float nearP = 0.1f;
    float farP = 250.0f;
    float focalPlaneR = focalPlane * farP;

    float CoC[2] = {
        ((apature * focalLength * focalPlaneR * (farP - nearP)) / ((focalPlaneR - focalLength) * nearP * farP)), //scale
        ((apature * focalLength * (nearP - focalPlaneR)) / ((focalPlaneR - focalLength) * nearP))    //bias
    };

    cbuffer.write(Global::context, CoC, sizeof(float) * 2);
}
