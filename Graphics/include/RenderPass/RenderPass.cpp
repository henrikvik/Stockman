#include "RenderPass.h"

namespace Graphics
{
    RenderPass::RenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil
    )
        : targets(targets)
        , resources(resources)
        , buffers(buffers)
        , depthStencil(depthStencil)
    {
    }
}