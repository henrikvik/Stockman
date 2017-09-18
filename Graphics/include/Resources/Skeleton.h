#pragma once
#include <Datatypes.h>
#include <vector>

namespace Graphics {
	class Skeleton
	{
	public:
		Skeleton();
		~Skeleton();

		void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);
		void release();

		void addSkeleton(
			unsigned int skeletonID,
			unsigned int vertexCount,
			UINT indexCount,
			vector<AnimVert> animVertices,
			vector<Vertex> vertices,
			vector<UINT> indices,
			bool isScene
		);

	private:
		ID3D11Device * gDevice = nullptr;
		ID3D11DeviceContext * gDeviceContext = nullptr;
	};
}