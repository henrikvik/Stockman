#include "ResourceManager.h"
#include <Engine\Constants.h>

namespace Graphics
{
    ResourceManager::ResourceManager()
    {

    }

    ResourceManager::~ResourceManager()
    {
		
    }

	void ResourceManager::initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
	{
		meshManager.initialize(gDevice, gDeviceContext);
		materialManager.initialize(gDevice, gDeviceContext);
		brfImporterHandler.initialize(meshManager, materialManager);

		brfImporterHandler.loadFile(CUBE, MODEL_PATH_STR("kubfixadtextur.brf"), true, true, false, false);
		brfImporterHandler.loadFile(SPHERE, MODEL_PATH_STR("sphere.brf"), true, true, false, false);
        brfImporterHandler.loadFile(CROSSBOW, MODEL_PATH_STR("CrossBow.brf"), true, true, false, false);
		brfImporterHandler.loadFile(AMMOBOX, MODEL_PATH_STR("ammoBox.brf"), true, true, false, false);
		brfImporterHandler.loadFile(CUTTLERY, MODEL_PATH_STR("cuttlery.brf"), true, true, false, false);
		brfImporterHandler.loadFile(JUMPPAD, MODEL_PATH_STR("jumpPad.brf"), true, true, false, false);
		brfImporterHandler.loadFile(ENEMYGRUNT, MODEL_PATH_STR("enemyGrunt.brf"), true, true, false, false);
		brfImporterHandler.loadFile(GRAPPLEPOINT, MODEL_PATH_STR("grapplePoint.brf"), true, true, false, false);
		brfImporterHandler.loadFile(GRASS, MODEL_PATH_STR("megaGrass.brf"), true, true, false, false);
		//brfImporterHandler.loadFile(BUSH, MODEL_PATH_STR("bushgreen.brf"), true, true, false, false);
		//brfImporterHandler.loadFile(CLOUDS, MODEL_PATH_STR("clouds.brf"), true, true, false, false);
		//brfImporterHandler.loadFile(MEGAGRASS, MODEL_PATH_STR("megaGrass.brf"), true, true, false, false);

		//brfImporterHandler.loadFile(FIREPIT, MODEL_PATH_STR("clouds.brf"), true, true, false, false);



		//brfImporterHandler.loadFile(MODEL_PATH_STR("kub2.brf"), true, true, false, false);
    }

    ModelInfo ResourceManager::getModelInfo(ModelID modelID)
    {
        Mesh * mesh = &meshManager.getMeshes()->at(modelID);


		ModelInfo info = {

			mesh->GetIndexCount(),
			mesh->getIndexBuffer(),
			mesh->getVertexBuffer()

        };

		materialManager.getMaterialInfo(info, modelID);
		

        return info;
    }

    void ResourceManager::release()
    {
        meshManager.release();
		materialManager.release();
    }
}