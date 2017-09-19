#pragma once
#include <memory>
#include "BRFImporterStructs.h"

namespace BRFImporterLib
{	
	class MeshContainer
	{
	public:
		std::shared_ptr<MeshHeader> meshData;
		std::unique_ptr<VertexHeaderNoSkeleton[]> vertexNoSkeletonData;
		std::unique_ptr<VertexHeader[]> vertexData;
		std::unique_ptr<IndexHeader[]> indexData;
		std::unique_ptr<WeigthsHeader[]> weightData;
		MeshContainer(unsigned int vertexCount, unsigned int indexCount);
		~MeshContainer();

	};
	class MeshData
	{
	protected:
		std::shared_ptr<MeshContainer> meshDataContainer;
	public:
		void SetData(std::shared_ptr<MeshContainer> SrcMeshData);


		MeshHeader* GetMeshData();
		VertexHeaderNoSkeleton GetVertexNoSkeletonData(unsigned int vert);
		VertexHeader  GetVertexData(unsigned int vert);
		IndexHeader	  GetIndexData( unsigned int ind);
		WeigthsHeader GetWeightData(unsigned int vert);
		//CON DECON
		MeshData();
		~MeshData();
	};
}