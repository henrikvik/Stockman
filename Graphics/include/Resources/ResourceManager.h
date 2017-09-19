#pragma once
#include "Mesh.h"
#include "MeshManager.h"
#include "BRFImportHandler.h"
#include "MaterialManager.h"
#include <Graphics\include\Structs.h>

namespace Graphics
{

	enum VertexShaderID
	{
		VERTEX_QUAD = 0,
		VERTEX_FORWARD_PLUS
	};

	enum GeometryShaderID
	{
	};

	enum PixelShaderID
	{
		PIXEL_QUAD = 0,
		PIXEL_FORWARD_PLUS
	};

	enum ComputeShaderID
	{
		COMPUTE_FRUSTUMS = 0,
		COMPUTE_CULL_GRIDS
	};

	enum SamplerID
	{
		pointSampler = 0
	};

	class ResourceManager
	{
	public:


		ResourceManager();
		~ResourceManager();

	void initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	void release();

    ModelInfo getModelInfo(ModelID modelID);


	private:
		MeshManager meshManager;
		BRFImportHandler brfImporterHandler;
		MaterialManager materialManager;

	};
}