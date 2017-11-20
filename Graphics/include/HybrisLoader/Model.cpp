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
    }


}