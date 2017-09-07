#pragma once
#include "ShaderHandler.h"

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

class ResourceManager
{
public:
	
	
	ResourceManager(ID3D11Device * device);
	~ResourceManager();
	
	void setShaders(VertexShaderID vertexID, GeometryShaderID geometryID, PixelShaderID pixelID, ID3D11DeviceContext* context);
	void setShaders(VertexShaderID vertexID, PixelShaderID pixelID, ID3D11DeviceContext* context);
	void setComputeShader(ComputeShaderID id);
	
private:

	ShaderHandler shaderHandler;

	std::vector<int> vertexShaders;
	std::vector<int> pixelShaders;
	std::vector<int> geometryShaders;
	std::vector<int> computeShaders;
};