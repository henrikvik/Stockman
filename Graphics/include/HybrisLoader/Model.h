#pragma once
#include <Hybris\Hybris.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <map>

#include "Skeleton.h"
#include "Animation.h"
#include "Mesh.h"
#include "Material.h"

namespace HybrisLoader
{
    class Model
    {
    public:
        Model(ID3D11Device * device, Hybris::File & file);
        
        size_t getVertexCount()  { return mesh.getVertexCount(); }

        StructuredBuffer<Vertex> & getVertexBuffer() { return mesh.getVertexBuffer(); }
        
        std::vector<SM::Matrix> getJointTransforms() { return skeleton.getJointTransforms(); }
        std::vector<SM::Matrix> evalAnimation(const char * animationName, float timeStamp) { return skeleton.evalAnimation(animationName, timeStamp); };
       
        float getAnimationDuration(const char * animationName) { return skeleton.getAnimationDuration(animationName); }


        Mesh & getMesh() { return mesh; }
        Material & getMaterial() { return material; }
        Skeleton & getSkeleton() { return skeleton; }

    private:
        Mesh mesh;
        Material material;
        Skeleton skeleton;

    };
}