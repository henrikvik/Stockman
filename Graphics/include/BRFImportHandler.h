#pragma once

#include "../../libs/BRFImporter/include/FileData.h"
#include "../../libs/BRFImporter/include/Fetch.h"
#include "../../libs/BRFImporter/include/MeshData.h"
#include "../../libs/BRFImporter/include/MaterialData.h"
#include "../../libs/BRFImporter/include/SkeletonData.h"

#include "Resources/MeshManager.h"
#include "Datatypes.h"
#include "Resources\Mesh.h"

class BRFImportHandler
{
public:
	BRFImportHandler();
	~BRFImportHandler();

	void loadFile(string fileName, bool mesh, bool material, bool skeleton, bool isScene);

	void initialize(MeshManager & meshManager/*, MaterialManager* materialManager*/);
	void release();

private:
	unsigned int materialID;

	BRFImporterLib::FileData* currentFile;
	MeshManager * meshManager;
	
};
