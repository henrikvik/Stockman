#pragma once
#include <Hybris\Hybris.h>
#include "../Utility/Texture.h"

namespace HybrisLoader
{
    class Material
    {
    public:
        Material(ID3D11Device * device, Hybris::Material & material);
        virtual ~Material();

        Texture & getDiffuse() { return *diffuse; }
        Texture & getSpecular() { return *specular; }
        Texture & getNormals() { return *normals; }
        Texture & getGlow() { return *glow; }

    private:
        Texture * diffuse;
        Texture * specular;
        Texture * normals;
        Texture * glow;

        void loadTexture(ID3D11Device * device, Hybris::Texture & src, Texture ** des);
    };
}
