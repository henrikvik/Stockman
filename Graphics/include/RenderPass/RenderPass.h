#pragma once
#include <CommonStates.h>

class RenderPass
{
public:
    virtual ~RenderPass();
    virtual void render() = 0;
    static DirectX::CommonStates * cStates;
};