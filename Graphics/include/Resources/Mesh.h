#pragma once

#include "../Datatypes.h"
#include <Engine\Constants.h>

class Mesh
{
public:
	unsigned int	animationCount = 0;

	Mesh(bool hasSkeleton, unsigned int skeletonID, unsigned int materialID);

	Mesh();
	~Mesh();

	void initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	void Release();

	unsigned int GetVertexCount() { return this->vertCount; };
	UINT GetIndexCount() { return this->indexCount; };

	int GetMaterialID() { return this->materialID; }
	void SetMaterialID(int id) { this->materialID = id; }

	unsigned int GetVertCount() { return this->vertCount; };
	bool GetIsScene() { return this->isScene; };
	Vertex* GetVertices() { return this->sceneVerts; };
	UINT* GetIndices() { return this->sceneIndex; };


	void CreateVertexBuffer(Vertex* vertices, unsigned int amount, bool isScene);

	void CreateIndexBuffer(UINT* indices, unsigned int amount, bool isScene);

    ID3D11Buffer* getVertexBuffer() { return vertexBuffer; };
    ID3D11Buffer* getIndexBuffer()  { return indexBuffer;  };
	
private:
	bool	        hasSkeleton = false;
	Vertex*		    vertices = nullptr;


	bool			isAnimated = false;

	bool	        isBlendShape = false;


	ID3D11Buffer*   vertexBuffer = nullptr;
	ID3D11Buffer*   indexBuffer = nullptr;
	unsigned int    vertCount = 0;
	UINT			indexCount = 0;

	unsigned int  skeletonID = 0;
	int  materialID = 0;

	Vertex* sceneVerts = nullptr;
	UINT* sceneIndex = nullptr;
	bool isScene = false;

	ID3D11DeviceContext* gDeviceContext; //a pointer to the device. (not sure if it will be needed her in the end)
	ID3D11Device *gDevice;
};