#pragma once

#include <FileData.h>
#include <Fetch.h>
#include <MeshData.h>
#include <MaterialData.h>
#include <SkeletonData.h>

#include <Resources/MeshManager.h>
#include <Datatypes.h>
#include <Resources\Mesh.h>

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
