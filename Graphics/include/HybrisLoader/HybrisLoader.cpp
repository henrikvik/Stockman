#include "HybrisLoader.h"
#include <fstream>
#include <Engine\Constants.h>
#include <Hybris\Read.h>


namespace HybrisLoader
{
    HybrisLoader::HybrisLoader(ID3D11Device * device)
    {
        size_t i = 0;
        for (auto & path : Resources::Models::Paths)
        {
            models[static_cast<Resources::Models::Files>(i++)] = new Model(device, loadFile(path));
        }
    }

    HybrisLoader::~HybrisLoader()
    {

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