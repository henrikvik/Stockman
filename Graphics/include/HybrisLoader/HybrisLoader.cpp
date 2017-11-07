#include "HybrisLoader.h"
#include <fstream>
#include <Engine\Constants.h>
#include <Hybris\Read.h>

namespace HybrisLoader
{
    HybrisLoader::HybrisLoader()
    {
        for (auto & path : Resources::Models::Paths)
        {
            Hybris::File file = loadFile(path.second);
            models[path.first] = newd Model(Global::device, file);
        }
    }

    HybrisLoader & HybrisLoader::get()
    {
        static HybrisLoader instance;
        return instance;
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