#include "DebugRenderPass.h"

Graphics::DebugRenderPass::DebugRenderPass(
    std::initializer_list<ID3D11RenderTargetView*> targets,
    std::initializer_list<ID3D11ShaderResourceView*> resources,
    std::initializer_list<ID3D11Buffer*> buffers,
    ID3D11DepthStencilView * depthStencil
)
    : RenderPass(targets, resources, buffers, depthStencil)
    , debugShader(Resources::Shaders::DebugShader)
    , vertexBuffer(CpuAccess::Write, INSTANCE_CAP(DebugRenderInfo))
{

}

Graphics::DebugRenderPass::~DebugRenderPass()
{
}

void Graphics::DebugRenderPass::render() const
{
    Global::context->VSSetShader(debugShader, nullptr, 0);
    Global::context->PSSetShader(debugShader, nullptr, 0);
    Global::context->OMSetRenderTargets(1, targets.data(), depthStencil);


    Global::context->VSSetConstantBuffers(0, 1, buffers.data());
    Global::context->VSSetConstantBuffers(1, 1, offsetBuffer);
    Global::context->VSSetShaderResources(0, 1, vertexBuffer);

    UINT offset = 0;
    for (auto & info : RenderQueue::get().getQueue<DebugRenderInfo>())
    {
        offsetBuffer.write(Global::context, &offset, sizeof(offset));
        offset += (UINT)info.points->size();

        Global::context->IASetPrimitiveTopology(info.topology);
        Global::context->OMSetDepthStencilState(info.useDepth ? Global::cStates->DepthDefault() : Global::cStates->DepthNone(), 0);
        Global::context->Draw((UINT)info.points->size(), 0);
    }

    for (auto & info : RenderQueue::get().getQueue<NewDebugRenderInfo>())
    {
        offsetBuffer.write(Global::context, &offset, sizeof(offset));
        offset += (UINT)info.points->size();

        Global::context->IASetPrimitiveTopology(info.topology);
        Global::context->OMSetDepthStencilState(info.useDepth ? Global::cStates->DepthDefault() : Global::cStates->DepthNone(), 0);
        Global::context->Draw((UINT)info.points->size(), 0);
    }
}

void Graphics::DebugRenderPass::update(float deltaTime)
{
    vertexBuffer.write([](DebugVertex * dest)
    {
        UINT points = 0;
        for (auto & info : RenderQueue::get().getQueue<DebugRenderInfo>())
        {
            for (auto & point : *info.points)
            {
                DebugVertex vertex;
                vertex.position = DirectX::SimpleMath::Vector4(point.x, point.y, point.z, 1);
                vertex.color = info.color;

                *dest++ = vertex;
                points++;

                if (points > INSTANCE_CAP(DebugRenderInfo)) return;
            }
        }

        for (auto & info : RenderQueue::get().getQueue<NewDebugRenderInfo>())
        {
            for (auto & point : *info.points)
            {
                DebugVertex vertex;
                vertex.position = DirectX::SimpleMath::Vector4(point.position.x, point.position.y, point.position.z, 1);
                vertex.color = point.color;

                *dest++ = vertex;
                points++;

                if (points > INSTANCE_CAP(DebugRenderInfo)) return;
            }
        }
    });
}
