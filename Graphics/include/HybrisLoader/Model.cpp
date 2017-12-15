#include "Model.h"

namespace HybrisLoader
{
    Model::Model(ID3D11Device * device, Hybris::FileWithHitbox & file)
        : skeleton(file.skeleton)
        , mesh(device, file.mesh)
        , material(device, file.material)
    {
        for (size_t i = 0; i < file.hitboxes.size; i++)
        {
            auto hbox = file.hitboxes[i];
            Hitbox hitbox;
            hitbox.position = DirectX::SimpleMath::Vector3(hbox.position);
            hitbox.rotation = DirectX::SimpleMath::Quaternion(hbox.rotation);
            hitbox.halfSize = DirectX::SimpleMath::Vector3(hbox.halfSize);
            hitboxes.push_back(hitbox);
        }

        for (size_t i = 0; i < file.mesh.vertices.size; i++)
        {
            bounding_box.max.x = std::fmax(file.mesh.vertices.data[i].position[0], bounding_box.max.x);
            bounding_box.max.y = std::fmax(file.mesh.vertices.data[i].position[1], bounding_box.max.y);
            bounding_box.max.z = std::fmax(file.mesh.vertices.data[i].position[2], bounding_box.max.z);
            bounding_box.min.x = std::fmin(file.mesh.vertices.data[i].position[0], bounding_box.min.x);
            bounding_box.min.y = std::fmin(file.mesh.vertices.data[i].position[1], bounding_box.min.y);
            bounding_box.min.z = std::fmin(file.mesh.vertices.data[i].position[2], bounding_box.min.z);

        }
    }


}