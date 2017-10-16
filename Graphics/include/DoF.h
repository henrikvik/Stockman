#pragma once
#include "Utility\ShaderResource.h"
#include "Resources\Shader.h"
#include "Camera.h"
#include <CommonStates.h>
#include "Utility\DepthStencil.h"

class DoF
{
public:
    DoF(ID3D11Device * device);
    ~DoF();

private:

    DirectX::CommonStates *states;
};
