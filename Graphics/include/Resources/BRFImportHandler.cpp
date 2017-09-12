#include <BRFImportHandler.h>

BRFImportHandler::BRFImportHandler()
{
	materialID = 0;
}

BRFImportHandler::~BRFImportHandler()
{

}

void BRFImportHandler::loadFile(string fileName, bool mesh, bool material, bool skeleton, bool isScene)
{
	this->currentFile->LoadFile(fileName, mesh, skeleton, material);

	unsigned int meshSize = currentFile->fetch->Main()->meshAmount;
	
	for (unsigned int i = 0; i < meshSize; i++)
	{
#pragma region handle material
		int tempMaterialID = 0;

		if (material == true)
		{
			tempMaterialID = currentFile->fetch->Material(i)->Id;
		}
		else
		{
			tempMaterialID = -1;
		}
#pragma endregion
#pragma region Statements handling vertices.

		unsigned int tempVertexCount = currentFile->fetch->Mesh(i)->GetMeshData()->vertexCount;
		vector <Vertex> tempVertices;
		
		for (unsigned int j = 0; j < tempVertexCount; j++)
		{
			Vertex tempVert;
			tempVert.position = {
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).pos[0],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).pos[1],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).pos[2],
			};
			tempVert.normal = {
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).normal[0],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).normal[1],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).normal[2],
			};
			tempVert.uv = {
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).uv[0],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).uv[1]
			};
			tempVert.tangent = {
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).tangent[0],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).tangent[1]
			};
			tempVert.biTangent = {
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).biTangent[0],
				(float)currentFile->fetch->Mesh(i)->GetVertexData(j).biTangent[1]
			};
			tempVertices.push_back(tempVert);
		}
		tempVertices.shrink_to_fit();
#pragma endregion

#pragma region statements handling indices

		UINT tempIndexCount = currentFile->fetch->Mesh(i)->GetMeshData()->indexCount;
		vector <UINT> tempIndices;
		for (unsigned int j = 0; j < tempIndexCount; j++)
		{
			UINT tempIndex;
			tempIndex = currentFile->fetch->Mesh(i)->GetIndexData(j).vertIndex;
			tempIndices.push_back(tempIndex);
		}
		tempIndices.shrink_to_fit();


		meshManager->addMesh(false, 0, 0, tempVertexCount, tempIndexCount, tempVertices, tempIndices, isScene);
		

	}

#pragma endregion

#pragma region ImportMaterials
	vector<importedMaterial> importedMaterials;
	vector<Mesh>* meshes = meshManager->GetMeshes();
	unsigned int materialSize = currentFile->fetch->Main()->materialAmount;

	for (unsigned int i = 0; i < materialSize; i++)
	{
		importedMaterial tempMaterial;
		tempMaterial.materialName = (std::string)currentFile->fetch->Material(i)->matName;

		tempMaterial.diffuseValue = {
			(float)currentFile->fetch->Material(i)->diffuseVal[0],
			(float)currentFile->fetch->Material(i)->diffuseVal[1],
			(float)currentFile->fetch->Material(i)->diffuseVal[1]
		};

		tempMaterial.specularValue = {
			(float)currentFile->fetch->Material(i)->specularVal[0],
			(float)currentFile->fetch->Material(i)->specularVal[1],
			(float)currentFile->fetch->Material(i)->specularVal[2]
		};

		tempMaterial.diffuseTex = (std::string)currentFile->fetch->Material(i)->diffMap;
		tempMaterial.specularTex = (std::string)currentFile->fetch->Material(i)->specMap;
		tempMaterial.normalTex = (std::string)currentFile->fetch->Material(i)->normalMap;
		tempMaterial.glowTex = (std::string)currentFile->fetch->Material(i)->glowMap;

		unsigned int tempMaterialID = currentFile->fetch->Material(i)->Id;
		tempMaterial.materialID = materialID;
		if (materialManager->compareImportMaterials(&tempMaterial))
		{
			unsigned int materialOffset = materialSize;
			for (unsigned int a = 0; a < meshes->size() - materialSize; a++)
			{
				unsigned int importMaterialID = meshes->at(a).GetMaterialID();
				if (materialManager->compareMaterials(&tempMaterial, importMaterialID))
				{
					tempMaterial.materialID = importMaterialID;
					meshes->at(meshes->size() - materialOffset).SetMaterialID(importMaterialID);
					materialOffset;
				}
			}
		}
		else
		{
			for (size_t j = meshes->size() - meshSize; j < meshes->size(); j++)
			{
				unsigned int importMaterialID = meshes->at(j).GetMaterialID();
				if (importMaterialID == tempMaterialID)
				{
					meshes->at(j).SetMaterialID(tempMaterial.materialID);
				}
			}
			materialID++;
		}
		importedMaterials.push_back(tempMaterial);
	}
	importedMaterials.shrink_to_fit();
	materialManager->addMaterials(&importedMaterials);
#pragma endregion
}

void BRFImportHandler::initialize(MeshManager & meshManager, MaterialManager & materialManager)
{
	this->currentFile = newd BRFImporterLib::FileData;
	this->meshManager = &meshManager;
	this->materialManager = &materialManager;
}

void BRFImportHandler::release()
{
	delete this->currentFile;
}
