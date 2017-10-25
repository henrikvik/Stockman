#include "Material.h"
#include <WICTextureLoader.h>

namespace HybrisLoader
{

    Material::Material(ID3D11Device * device, Hybris::Material & material)
    {
        if (material.diffuseMap.data.size > 0)
        {
            std::vector<uint8_t> data(
                &material.diffuseMap.data[0],
                &material.diffuseMap.data[
                    material.diffuseMap.data.size
                ]
            );
            diffuse = new Texture(device, data);
        }
        else
        {
            diffuse = new Texture();
        }

        if (material.specularMap.data.size > 0)
        {
            std::vector<uint8_t> data(
                &material.specularMap.data[0], 
                &material.specularMap.data[
                    material.specularMap.data.size
                ]
            );
            specular = new Texture(device, data);
        }
        else
        {
            specular = new Texture();
        }

        if (material.normalMap.data.size > 0)
        {
            std::vector<uint8_t> data(
                &material.normalMap.data[0], 
                &material.normalMap.data[
                    material.normalMap.data.size
                ]
            );
            normals = new Texture(device, data);
        }
        else
        {
            normals = new Texture();
        }

        if (material.glowMap.data.size > 0)
        {
            std::vector<uint8_t> data(
                &material.glowMap.data[0], 
                &material.glowMap.data[
                    material.glowMap.data.size
                ]
            );
            glow = new Texture(device, data);
        }
        else
        {
            glow = new Texture();
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