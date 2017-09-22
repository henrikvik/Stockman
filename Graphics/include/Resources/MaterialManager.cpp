#include "MaterialManager.h"

namespace Graphics
{
	MaterialManager::MaterialManager()
	{
		textureManager = newd TextureManager;
		materials = newd vector<Material>;;
	}


	MaterialManager::~MaterialManager()
	{
		delete materials;
		delete textureManager;
	}

	void MaterialManager::initialize(ID3D11Device * gDevice, ID3D11DeviceContext* gDeviceContext)
	{
		textureManager->initilize(gDevice);
	}

	void MaterialManager::release()
	{
		this->textureManager->release();
	}

	bool MaterialManager::compareMaterials(importedMaterial * import, unsigned int materialID)
	{
		if (import->materialName == materials->at(materialID).materialName)
		{
			if (materials->at(materialID).diffuse_ID == textureManager->findDiffuseID(import->diffuseTex))
			{
				if (materials->at(materialID).normal_ID == textureManager->findNormalID(import->normalTex))
				{
					if (materials->at(materialID).specular_ID == textureManager->findSpeularID(import->specularTex))
					{
						if (materials->at(materialID).glow_ID == textureManager->findGloowID(import->glowTex))
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	bool MaterialManager::compareImportMaterials(importedMaterial * import)
	{
		for (unsigned int i = 0; i < materials->size(); i++)
		{
			if (import->materialName == materials->at(i).materialName)
			{
				if (materials->at(i).diffuse_ID == textureManager->findDiffuseID(import->diffuseTex))
				{
					if (materials->at(i).normal_ID == textureManager->findNormalID(import->normalTex))
					{
						if (materials->at(i).specular_ID == textureManager->findSpeularID(import->specularTex))
						{
							if (materials->at(i).glow_ID == textureManager->findGloowID(import->glowTex))
							{
								return true;
							}
						}
					}
				}
			}
		}

		return false;
	}

	void MaterialManager::addMaterials(vector<importedMaterial>* import)
	{
		for (unsigned int i = 0; i < import->size(); i++)
		{
			if (!compareImportMaterials(&import->at(i)))
			{
				Material tempMat;

				tempMat.materialName = import->at(i).materialName;
				tempMat.materialID = import->at(i).materialID;

				tempMat.diffuseValue = import->at(i).diffuseValue;
				tempMat.specularValue = import->at(i).specularValue;

				tempMat.diffuse_ID = textureManager->getDiffuseID(import->at(i).diffuseTex);
				tempMat.normal_ID = textureManager->getNormalID(import->at(i).normalTex);
				tempMat.specular_ID = textureManager->getSpecularID(import->at(i).specularTex);
				tempMat.glow_ID = textureManager->getGlowID(import->at(i).glowTex);

				materials->push_back(tempMat);
				materials->shrink_to_fit();
			}
			else
			{
				Material tempMat;

				tempMat.materialName = import->at(i).materialName;
				tempMat.materialID = import->at(i).materialID;

				tempMat.diffuseValue = import->at(i).diffuseValue;
				tempMat.specularValue = import->at(i).specularValue;

				tempMat.diffuse_ID = textureManager->getDiffuseID(import->at(i).diffuseTex);
				tempMat.normal_ID = textureManager->getNormalID(import->at(i).normalTex);
				tempMat.specular_ID = textureManager->getSpecularID(import->at(i).specularTex);
				tempMat.glow_ID = textureManager->getGlowID(import->at(i).glowTex);

				materials->push_back(tempMat);
			}
		}
	}

	void MaterialManager::getMaterialInfo(ModelInfo & modelInfo, int iD)
	{
		modelInfo.diffuseMap = textureManager->GetDiffuseTexture(materials->at(iD).diffuse_ID);
		modelInfo.normalMap = textureManager->GetNormalTexture(materials->at(iD).normal_ID);
		modelInfo.specularMap = textureManager->GetSpecularTexture(materials->at(iD).specular_ID);
	}

}