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

	}
#pragma endregion
#pragma region

	vector<importedMaterial> importedMaterials;
	vector<Mesh>* meshes = meshManager->GetMeshes();

}

void BRFImportHandler::initialize(MeshManager * meshManager)
{
	this->currentFile = new BRFImporterLib::FileData;
	this->meshManager = meshManager;
}

void BRFImportHandler::release()
{
	delete this->currentFile;
}
