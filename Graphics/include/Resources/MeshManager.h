#pragma once

#include <Includes.h>
#include <Datatypes.h>
#include <Resources/Mesh.h>

class MeshManager
{
public:
	MeshManager();
	~MeshManager();
	void initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	void release();

	void addMesh(
		bool hasSkeleton,
		unsigned int skeletonID,
		int materialID,
		unsigned int vertexCount,
		UINT indexCount,
		vector<Vertex> vertices,
		vector<UINT> indices,
		bool isScene
	);

	vector<Mesh>* GetMeshes() { return this->gameMeshes; }

private:
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext *gDeviceContext = nullptr;

	vector<Mesh>* gameMeshes = nullptr;
	vector<Mesh>* sceneMeshes = nullptr;


};

