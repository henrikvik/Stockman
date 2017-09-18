#pragma once

#include "FileData.h"
#include "Fetch.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "SkeletonData.h"

#include "MeshManager.h"
#include "MaterialManager.h"
#include <Graphics\Include\Datatypes.h>
#include "Mesh.h"

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
