#include "Model.h"

namespace HybrisLoader
{
    Model::Model(ID3D11Device * device, Hybris::File & file)
        : skeleton(file.skeleton)
        , mesh(device, file.mesh)
        , material(device, file.material)
    {
    }


}