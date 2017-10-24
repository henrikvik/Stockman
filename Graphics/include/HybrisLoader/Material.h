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

    private:
        Texture * diffuse;
        Texture * specular;
        Texture * normals;
        Texture * glow;
    };
}
