#pragma once
#include <Engine\Constants.h>
#include "Resources\Shader.h"
#include "Utility\StructuredBuffer.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>

using namespace Graphics;

class Fog
{
public:
	Fog(ID3D11Device* device);
	~Fog();

	void renderFog(ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView * backBuffer);

private:
	struct FogData
	{
		DirectX::SimpleMath::Vector3 pos;
		DirectX::SimpleMath::Color color;

	};
	std::vector<FogData> fogVec;
	Shader fogShader;
	StructuredBuffer<Fog::FogData> fogDataBuffer;

};