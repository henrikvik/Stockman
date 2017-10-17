#pragma once
#include <map>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Model.h"

#include <Hybris\Hybris.h>

namespace HybrisLoader
{
    enum class FileID
    {
        BakedTest,
        AnimationWithLayers,
        Animation1Layer,
        Oriented,
        Golem,
        GolemOrigo,
        Cylinder
    };

    class HybrisLoader
    {
    public:
        HybrisLoader(ID3D11Device * device);
        virtual ~HybrisLoader();

        Model * getModel(FileID fileId) { return models.at(fileId); }
    private:
        std::map<FileID, Model*> models;

        Hybris::File loadFile(const char * filePath);
    };
}