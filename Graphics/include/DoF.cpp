#include "DoF.h"

DoF::DoF(ID3D11Device * device)
{

    states = new DirectX::CommonStates(device);
}

DoF::~DoF()
{
}
