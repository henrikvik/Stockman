#include "DoF.h"
namespace Graphics
{
    DoF::DoF(ID3D11Device * device)
    {

        states = new DirectX::CommonStates(device);
        samplers[0] = states->PointClamp();
        samplers[1] = states->LinearClamp();

    }

    DoF::~DoF()
    {
        SAFE_RELEASE(samplers[0]);
        SAFE_RELEASE(samplers[1]);
        delete states;
    }

    void DoF::DoFRender(ID3D11DeviceContext * context, ShaderResource * colorBuffer, DepthStencil * depthBuffer, ShaderResource * outputBuffer)
    {


    }
}
