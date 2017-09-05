#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

namespace Graphics
{
    static DirectX::SimpleMath::Vector2 FSQuadVerts[4] = {
        { -1, -1 },
        { -1,  1 },
        { 1, -1 },
        { 1,  1 }
    };

    static DirectX::SimpleMath::Vector2 TriangleVerts[3] = {
        { 0, -0.5 },
        { -0.5, 0.5 },
        { 0.5, 0.5 }
    };
};