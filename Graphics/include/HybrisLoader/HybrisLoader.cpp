#include "HybrisLoader.h"
#include <fstream>
#include <Engine\Constants.h>
#include <Hybris\Read.h>

namespace HybrisLoader
{
    HybrisLoader::HybrisLoader(ID3D11Device * device)
    {
        for (auto & path : Resources::Models::Paths)
        {
            Hybris::File file = loadFile(path.second);
            models[path.first] = newd Model(device, file);
        }
    }

    HybrisLoader::~HybrisLoader()
    {

        for (auto & model : models)
        {
            delete model.second;
        }
    }

    Hybris::File HybrisLoader::loadFile(const char * filePath)
    {
        std::ifstream ifile(filePath, std::ios::binary);
        Hybris::File hfile = {};
        Hybris::read(ifile, hfile);
        ifile.close();
        return hfile;
    }
}