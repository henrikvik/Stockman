#pragma once
#include "Utility\ShaderResource.h"
#include "Resources\Shader.h"
#include "Camera.h"
#include <CommonStates.h>
#include "Utility\DepthStencil.h"
namespace Graphics
{
	class SSAORenderer
	{
	public:
		SSAORenderer(ID3D11Device * device);
		~SSAORenderer();
		void renderSSAO(ID3D11DeviceContext * context, Camera * camera, DepthStencil * depthBuffer, ShaderResource * inputBackBuffer, ShaderResource * outputBackBuffer);

		ShaderResource * getNormalShaderResource() { return &normalTexture; }

	private:
		ShaderResource ssaoOutput;
		ShaderResource ssaoOutputSwap;
		ComputeShader ssaoShader;
		ComputeShader blurHorizontal;
		ComputeShader blurVertical;
		ComputeShader ssaoMerger;
		ShaderResource normalTexture;
		ID3D11ShaderResourceView * randomNormals;

		DirectX::CommonStates * states;
	};
}