#pragma once

#include <BRF\FileData.h>
#include <BRF\Fetch.h>
#include <BRF\MeshData.h>
#include <BRF\MaterialData.h>
#include <BRF\SkeletonData.h>

#include "MeshManager.h"
#include "MaterialManager.h"
#include <Graphics\Include\Datatypes.h>
#include "Mesh.h"
namespace Graphics
{
	class BRFImportHandler
	{
	public:
		BRFImportHandler();
		~BRFImportHandler();

		void loadFile(string fileName, bool mesh, bool material, bool skeleton, bool isScene);

		void initialize(MeshManager & meshManager, MaterialManager & materialManager);

	private:
		unsigned int materialID;

		BRFImporterLib::FileData* currentFile;
		MeshManager * meshManager;
		MaterialManager* materialManager;

	};
}
