#pragma once
#include <CommonStates.h>

class RenderPass
{
public:
    virtual void render() const = 0;
    virtual void update(float deltaTime) = 0;
    static DirectX::CommonStates * cStates;
};