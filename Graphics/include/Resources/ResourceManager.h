#pragma once
#include "ShaderHandler.h"
#include "Resources/Mesh.h"
#include <Resources/MeshManager.h>
#include <BRFImportHandler.h>
#include <Resources\MaterialManager.h>


#define NROFSAMPLERS 1
enum VertexShaderID
{
	VERTEX_QUAD = 0,
	VERTEX_DEFFERED = 1
};

enum GeometryShaderID
{
};

enum PixelShaderID
{
	PIXEL_QUAD = 0,
	PIXEL_DEFFERED = 1
};

enum ComputeShaderID
{
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
	
	void setShaders(VertexShaderID vertexID, GeometryShaderID geometryID, PixelShaderID pixelID, ID3D11DeviceContext* context);
	void setShaders(VertexShaderID vertexID, PixelShaderID pixelID, ID3D11DeviceContext* context);
	void setComputeShader(ComputeShaderID id);

	void setSampler(SamplerID id, ID3D11DeviceContext* context);
	
	void initShaders(ID3D11Device *gdevice);
	
private:
	void createSamplerState(ID3D11Device* device);

	ShaderHandler shaderHandler;

	ID3D11SamplerState* sampleStates[NROFSAMPLERS];

	MeshManager meshManager;
	BRFImportHandler brfImporterHandler;
	MaterialManager materialManager;

};