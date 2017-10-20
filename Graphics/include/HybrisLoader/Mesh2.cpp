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
        vertexBuffer = new StructuredBuffer<Vertex>(device, CpuAccess::None, vertices.size(), vertices.data());
    }

    Mesh::~Mesh()
    {
        delete vertexBuffer;
    }

    Vertex::Vertex(Hybris::Vertex & vertex)
    {
        position = Vector3(vertex.position);
        normal = Vector3(vertex.normal);
        binormal = Vector3(vertex.binormal);
        tangent = Vector3(vertex.tangent);
        uv = Vector2(vertex.uv);

        for (size_t i = 0; i < 4; i++)
        {
            jointIds[i] = vertex.jointIds[i];
            jointWeights[i] = vertex.jointWeights[i];
        }
    }
}