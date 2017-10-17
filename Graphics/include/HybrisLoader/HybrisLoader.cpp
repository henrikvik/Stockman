#include "HybrisLoader.h"
#include <fstream>
#include <Engine\Constants.h>
#include <Hybris\Read.h>

namespace HybrisLoader
{
    HybrisLoader::HybrisLoader(ID3D11Device * device)
    {
        models[FileID::BakedTest]           = new Model(device, loadFile(ANIMATION_PATH("BakedTest.hyb")));
        //models[FileID::AnimationWithLayers] = new Model(device, loadFile(ANIMATION_PATH("AnimationWithLayers.hyb")));
        //models[FileID::Animation1Layer]     = new Model(device, loadFile(ANIMATION_PATH("AnimationLayers.hyb")));
        //models[FileID::Oriented]     = new Model(device, loadFile(ANIMATION_PATH("Oriented.hyb")));
        //models[FileID::Golem]     = new Model(device, loadFile(ANIMATION_PATH("KittyGolemAnim.hyb")));
        //models[FileID::GolemOrigo]     = new Model(device, loadFile(ANIMATION_PATH("KittyGolemAnimOrigo.hyb")));
        models[FileID::Cylinder]     = new Model(device, loadFile(ANIMATION_PATH("Cylinder.hyb")));
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