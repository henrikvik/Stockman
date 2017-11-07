#pragma once

#include "RenderPass.h"

class ShadowRenderPass : public RenderPass
{
public:
    ShadowRenderPass(ID3D11DepthStencilView * shadowMap, D3D11_VIEWPORT viewport, ID3D11Buffer * lightMatrixes);
    virtual ~ShadowRenderPass() {};

    void update(float deltaTime);
    void render();

private:


};