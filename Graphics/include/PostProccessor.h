#pragma once
#include "Resources\Shader.h"
#include <Engine\Constants.h>
#include "Resources\ShaderResource.h"
#include <CommonStates.h>
#include "Utility\ConstantBuffer.h"
#include <vector>

namespace Graphics
{
	class PostProcessor
	{
	public:
		PostProcessor(ID3D11Device * device, ID3D11DeviceContext * context);
		~PostProcessor();

		void addGlow(ID3D11DeviceContext * context, ID3D11ShaderResourceView * backBuffer, ID3D11ShaderResourceView * glowMap, ShaderResource * outputTexture);

		std::vector<float> generateKernel(int kernelSize, float sigma);
	private:
		ComputeShader glow;
		ComputeShader glow2;
		ComputeShader merger;

		ShaderResource glowPass0;
		ShaderResource glowPass1;

		DirectX::CommonStates * states;
	};
}