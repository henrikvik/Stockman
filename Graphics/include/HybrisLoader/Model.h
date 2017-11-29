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
        struct Hitbox
        {
            DirectX::SimpleMath::Vector3 position;
            DirectX::SimpleMath::Quaternion rotation;
            DirectX::SimpleMath::Vector3 halfSize;
        };

        Model(ID3D11Device * device, Hybris::FileWithHitbox & file);
        
        size_t getVertexCount()  { return mesh.getVertexCount(); }

        StructuredBuffer<Vertex> & getVertexBuffer() { return mesh.getVertexBuffer(); }
        
        std::vector<DirectX::SimpleMath::Matrix> getJointTransforms() { return skeleton.getJointTransforms(); }
        std::vector<DirectX::SimpleMath::Matrix> evalAnimation(const char * animationName, float timeStamp) { return skeleton.evalAnimation(animationName, timeStamp); };
       
        float getAnimationDuration(const char * animationName) { return skeleton.getAnimationDuration(animationName); }

        Mesh & getMesh() { return mesh; }
        Material & getMaterial() { return material; }
        Skeleton & getSkeleton() { return skeleton; }
        std::vector<Hitbox> * getHitboxes() { return &hitboxes; }


        struct BoundingBox
        {
            DirectX::SimpleMath::Vector3 min;
            DirectX::SimpleMath::Vector3 max;

            void apply_scale(DirectX::SimpleMath::Vector3 & scale)
            {
                min *= scale;
                max *= scale;
            }

            virtual const float sphere_radius() const noexcept final
            {
                return (max - min).Length() * 0.5f;
            }
        };
        BoundingBox get_bounding_box() { return bounding_box; }
        
    private:
        Mesh mesh;
        Material material;
        Skeleton skeleton;
        std::vector<Hitbox> hitboxes;
        BoundingBox bounding_box;

    };
}