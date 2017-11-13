#include "Material.h"
#include <WICTextureLoader.h>
#include <Engine\newd.h>

namespace HybrisLoader
{

    Material::Material(ID3D11Device * device, Hybris::Material & material)
    {
        loadTexture(device, material.diffuseMap, &diffuse);
        loadTexture(device, material.specularMap, &specular);
        loadTexture(device, material.normalMap, &normals);
        loadTexture(device, material.glowMap, &glow);
    }

    Material::~Material()
    {
        delete diffuse;
        delete specular;
        delete normals;
        delete glow;
    }
    void Material::loadTexture(ID3D11Device * device, Hybris::Texture & src, Texture ** des)
    {
        if (src.data.size > 0)
        {
            std::vector<uint8_t> data(
                &src.data[0],
                &src.data[src.data.size]
            );

            *des = newd Texture(device, data, strcmp(src.extension, ".dds") == 0 ? true : false);
        }
        else
        {
            *des = newd Texture();
        }
    }
}