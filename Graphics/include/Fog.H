#pragma once
#include <Engine\Constants.h>
#include "Resources\Shader.h"
#include "Utility\StructuredBuffer.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>

namespace Graphics
{
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
			DirectX::SimpleMath::Vector4 color;

		};
		Shader fogShader;
		std::vector<FogData> fogData;
		StructuredBuffer<Fog::FogData> fogDataBuffer;

	};
}
