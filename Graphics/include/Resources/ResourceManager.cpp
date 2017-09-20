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

		brfImporterHandler.loadFile(MODEL_PATH_STR("kubenmedtextur.brf"), true, true, false, false);
		//brfImporterHandler.loadFile(MODEL_PATH_STR("kub2.brf"), true, true, false, false);
    }

    ModelInfo ResourceManager::getModelInfo(ModelID modelID)
    {
        Mesh & mesh = meshManager.GetMeshes()->at(modelID);


        ModelInfo info = {
	
            mesh.GetIndexCount(),
            mesh.getIndexBuffer(),
            mesh.getVertexBuffer()
        };

        return info;
    }

    void ResourceManager::release()
    {
        meshManager.release();
    }
}