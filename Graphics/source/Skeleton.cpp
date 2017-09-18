#include <resources/Skeleton.h>

namespace Graphics {
	Skeleton::Skeleton()
	{

	}

	Skeleton::~Skeleton()
	{

	}

	void Skeleton::initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
	{
		this->gDevice = gDevice;
		this->gDeviceContext = gDeviceContext;
	}

	void Skeleton::release()
	{

	}

	void Skeleton::addSkeleton(
		unsigned int skeletonID,
		unsigned int vertexCount,
		UINT indexCount,
		vector<AnimVert> animVertices,
		vector<Vertex> vertices,
		vector<UINT> indices,
		bool isScene)
	{

	}
}