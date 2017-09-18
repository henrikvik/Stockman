#pragma once
#include <vector>
#include <Graphics\include\Datatypes.h>
#include "TextureManager.h"

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();
	void initialize(ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext);
	void release();

	bool compareMaterials(importedMaterial* import, unsigned int materialID);
	bool compareImportMaterials(importedMaterial* import);
	void addMaterials(vector<importedMaterial>* import);

private:
	std::vector<Material>* materials;
	TextureManager* textureManager;
};

