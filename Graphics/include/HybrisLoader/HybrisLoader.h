#pragma once
#include <map>
#include <memory>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Model.h"

#include <Hybris\Hybris.h>
#include <Resources\Resources.h>

namespace HybrisLoader
{

    class HybrisLoader
    {
    public:
        HybrisLoader(ID3D11Device * device);
        virtual ~HybrisLoader();

        Model * getModel(Resources::Models::Files fileId) { return models.at(fileId); }
    private:
        std::map<Resources::Models::Files, Model*> models;

        Hybris::File loadFile(const char * filePath);
    };
}