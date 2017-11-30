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
        static const std::initializer_list<D3D11_INPUT_ELEMENT_DESC> INPUT_DESC;
        static const UINT STRIDE;

        using Vector2 = DirectX::SimpleMath::Vector2;
        using Vector3 = DirectX::SimpleMath::Vector3;

        Vertex(Hybris::Vertex & vertex);

        Vector3  position;
        Vector3  normal;
        Vector3  binormal;
        Vector3  tangent;
        Vector2  uv;

        uint8_t jointIds[4];
        float   jointWeights[4];
    };

    class Mesh
    {
    public:
        Mesh(ID3D11Device * device, Hybris::Mesh & mesh);
        virtual ~Mesh();

        size_t getVertexCount() { return vertexCount; }
        
        Buffer<Vertex> & getVertexBuffer() { return *vertexBuffer; }
    private:
        size_t vertexCount;
        Buffer<Vertex> * vertexBuffer;
    };
}