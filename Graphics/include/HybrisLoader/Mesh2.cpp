#include "Mesh.h"

namespace HybrisLoader
{
    Mesh::Mesh(ID3D11Device * device, Hybris::Mesh & mesh)
    {
        std::vector<Vertex> vertices;
        for (size_t i = 0; i < mesh.vertices.size; i++)
        {
            vertices.emplace_back(mesh.vertices[i]);
        }

        vertexCount = vertices.size();
        vertexBuffer = newd Buffer<Vertex>(
            BufferUsage::Immutable, 
            BufferBind::VertexBuffer, 
            BufferCpuAccess::None, 
            BufferMisc::None, 
            vertices.size(), 
            vertices.data()
        );
    }

    Mesh::~Mesh()
    {
        delete vertexBuffer;
    }

    const std::initializer_list<D3D11_INPUT_ELEMENT_DESC> Vertex::INPUT_DESC =
    {
        { "POSITION",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",        0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",       0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "UV",            0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "JOINT_IDS",     0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "JOINT_WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    const UINT Vertex::STRIDE = sizeof(Vertex);

    Vertex::Vertex(Hybris::Vertex & vertex)
    {
        position = Vector3(vertex.position);
        normal   = Vector3(vertex.normal);
        binormal = Vector3(vertex.binormal);
        tangent  = Vector3(vertex.tangent);
        uv       = Vector2(vertex.uv);

        for (size_t i = 0; i < 4; i++)
        {
            jointIds[i] = vertex.jointIds[i];
            jointWeights[i] = vertex.jointWeights[i];
        }
    }
}