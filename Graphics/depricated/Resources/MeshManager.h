#pragma once
#include <vector>
#include <map>
#include <Graphics\include\Datatypes.h>
#include "Mesh.h"
namespace Graphics
{
	using namespace std;

	class MeshManager
	{
	public:
		MeshManager();
		~MeshManager();
		void initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
		void release();

		void addMesh(
			int id,
			bool hasSkeleton,
			unsigned int skeletonID,
			int materialID,
			unsigned int vertexCount,
			UINT indexCount,
			vector<Vertex> vertices,
			vector<UINT> indices,
			bool isScene
		);

		Mesh * getMesh(int id) { return gameMeshes[id]; }
		vector<Mesh>* getMeshes() { return &meshes; }

	private:
		ID3D11Device *gDevice = nullptr;
		ID3D11DeviceContext *gDeviceContext = nullptr;

		map<int,Mesh*> gameMeshes;
		vector<Mesh> meshes;
		vector<Mesh> sceneMeshes;
		Mesh mesh;


	};

}