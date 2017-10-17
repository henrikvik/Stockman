#pragma once
#include <Hybris\Hybris.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <map>

#include "Skeleton.h"
#include "Animation.h"
#include "Mesh.h"

namespace HybrisLoader
{
    class Model
    {
    public:
        Model(ID3D11Device * device, Hybris::File & file);
        
        StructuredBuffer<Vertex> & getVertexBuffer() { return mesh.getVertexBuffer(); }
        Buffer<uint32_t> &         getIndexBuffer()  { return mesh.getIndexBuffer(); }
        size_t                     getIndexCount()   { return mesh.getIndexCount(); }

        std::vector<SM::Matrix> evalAnimation(const char * animationName, float timeStamp) { return skeleton.evalAnimation(animationName, timeStamp); };
    private:

        Mesh mesh;
        Skeleton skeleton;
    };
}