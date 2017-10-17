#pragma once
#include <Hybris\Hybris.h>
#include <d3d11.h>
#include <SimpleMath.h>

#include "../Utility/StructuredBuffer.h"
#include "../Utility/Buffer.h"

namespace HybrisLoader
{
    struct Vertex
    {
        using Vector2 = DirectX::SimpleMath::Vector2;
        using Vector3 = DirectX::SimpleMath::Vector3;

        Vertex(Hybris::Vertex & vertex);

        Vector3  position;
        Vector3  normal;
        Vector3  binormal;
        Vector3  tangent;
        Vector2  uv;

        uint32_t jointIds[4];
        float    jointWeights[4];
    };

    class Mesh
    {
    public:
        Mesh(ID3D11Device * device, Hybris::Mesh & mesh);
        virtual ~Mesh();

        StructuredBuffer<Vertex> & getVertexBuffer() { return *vertexBuffer; }
        Buffer<uint32_t> & getIndexBuffer()          { return *indexBuffer; }
        uint32_t getIndexCount()                     { return indexCount; }
    private:
        StructuredBuffer<Vertex> * vertexBuffer;
        Buffer<uint32_t> * indexBuffer;
        uint32_t indexCount;
    };
}