#pragma once
#include <initializer_list>
#include <d3d11.h>
#include "../Device.h"
#include <CommonStates.h>
#include <vector>

class RenderPass
{
public:
    static DirectX::CommonStates * cStates;
    virtual void render() = 0;
};