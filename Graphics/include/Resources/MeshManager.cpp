#include "MeshManager.h"

namespace Graphics
{
	MeshManager::MeshManager()
	{
	}


	MeshManager::~MeshManager()
	{
	}

	void MeshManager::initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
	{
		this->gDevice = gDevice;
		this->gDeviceContext = gDeviceContext;

	}

	void MeshManager::release()
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes.at(i).Release();
		}
		for (size_t i = 0; i < sceneMeshes.size(); i++)
		{
			sceneMeshes.at(i).Release();
		}
	}

	void MeshManager::addMesh(int id, bool hasSkeleton, unsigned int skeletonID, int materialID, unsigned int vertexCount, UINT indexCount, vector<Vertex> vertices, vector<UINT> indices, bool isScene)
	{
		Vertex* newVertices = new Vertex[vertexCount];
		for (unsigned int i = 0; i < vertexCount; i++)
		{
			newVertices[i] = vertices[i];
		}

		UINT* newIndices = new UINT[indexCount];
		for (unsigned int i = 0; i < indexCount; i++)
		{
			newIndices[i] = indices[i];
		}
		Mesh newMesh = Mesh(hasSkeleton, skeletonID, materialID);
		newMesh.initialize(this->gDevice, this->gDeviceContext);
		newMesh.CreateVertexBuffer(newVertices, vertexCount, isScene);
		newMesh.CreateIndexBuffer(newIndices, indexCount, isScene);
		if (isScene == true)
			this->sceneMeshes.push_back(newMesh);
		else
		{
			meshes.push_back(newMesh);
			this->gameMeshes.insert_or_assign(id, &meshes[meshes.size() - 1]);
			delete[] newVertices;
			delete[] newIndices;

		}

	}
}