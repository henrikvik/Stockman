#pragma once
#include "Resources\Shader.h"
#include <Engine\Constants.h>
#include "Utility\ShaderResource.h"
#include <CommonStates.h>
#include "Utility\ConstantBuffer.h"
#include <vector>
#define MIP_LEVELS 5

namespace Graphics
{
	class Glow
	{
	public:
		Glow(ID3D11Device * device, ID3D11DeviceContext * context);
		~Glow();

		void addGlow(ID3D11DeviceContext * context, ID3D11ShaderResourceView * backBuffer, ShaderResource * outputTexture);

		std::vector<float> generateKernel(int kernelSize, float sigma);

		void recompileGlow(ID3D11Device * device);

		operator ID3D11RenderTargetView*const*() { return &rtvs[0]; };
		operator ID3D11RenderTargetView*() { return rtvs[0]; };

		void clear(ID3D11DeviceContext * context, float color[4]);
	private:
		Shader glow;
		Shader glow2;
		Shader merger;
		Shader mipGenerator;
		Shader mipCombinder;

		ID3D11ShaderResourceView * srvs[MIP_LEVELS];
		ID3D11RenderTargetView * rtvs[MIP_LEVELS];

		ID3D11ShaderResourceView * srvAllMips;

		ShaderResource glowPass0;
		ShaderResource glowPass1;

		DirectX::CommonStates * states;

		void createMips(ID3D11Device * device);
		void setMipViewPort(ID3D11DeviceContext * context, int level);
	};
}