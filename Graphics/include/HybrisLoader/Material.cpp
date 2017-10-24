#include "Material.h"
#include <WICTextureLoader.h>

namespace HybrisLoader
{

    Material::Material(ID3D11Device * device, Hybris::Material & material)
    {
        if (material.diffuseMap.data.size > 0)
        {
            std::vector<uint8_t> data(&material.diffuseMap.data[0], &material.diffuseMap.data[material.diffuseMap.data.size]);
            diffuse = new Texture(device, data);
        }

       

    }

    Material::~Material()
    {
        delete diffuse;
        delete specular;
        delete normals;
        delete glow;
    }
}