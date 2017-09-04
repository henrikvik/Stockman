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

    static  ID3D11Buffer * FSQuad;
    static  ID3D11Buffer * Triangle;

    void InitializeConstants(ID3D11Device * device)
    {
        D3D11_BUFFER_DESC bufferDesc = { 0 };
        bufferDesc.ByteWidth = sizeof(FSQuadVerts);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = FSQuadVerts;

        device->CreateBuffer(&bufferDesc, &data, &FSQuad);

        bufferDesc.ByteWidth = sizeof(TriangleVerts);
        data.pSysMem = TriangleVerts;

        device->CreateBuffer(&bufferDesc, &data, &Triangle);
    }
};