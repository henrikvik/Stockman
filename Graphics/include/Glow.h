#pragma once
#include <vector>
#include <CommonStates.h>
#include <Engine\Constants.h>
#include "Utility\Shader.h"
#include "Utility\ShaderResource.h"
#include "Utility\ConstantBuffer.h"

namespace Graphics
{
	class Glow
	{
	public:
		Glow(ID3D11Device * device, ID3D11DeviceContext * context);
		~Glow();

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