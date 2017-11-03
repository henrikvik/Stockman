#pragma once
#include <CommonStates.h>

class RenderPass
{
public:
    virtual void render() = 0;
    virtual void update(float deltaTime) {};
    static DirectX::CommonStates * cStates;
};