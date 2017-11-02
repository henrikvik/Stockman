#pragma once
#include "Resources\Shader.h"
#include "Utility\ConstantBuffer.h"
#include "Camera.h"
#include "Lights\Sun.h"
#include "Utility\DepthStencil.h"
#include "Datatypes.h"
#include "Structs.h"
#define SHADOW_MAP_RESOLUTION 2048

namespace Graphics
{
	class SkyRenderer
	{
	public:
		SkyRenderer(ID3D11Device * device, int shadowRes);
		~SkyRenderer();

		void initialize(ModelInfo info);

		void renderSky(ID3D11DeviceContext * context, Graphics::Camera * cam);
		void update(ID3D11DeviceContext * context, float deltaTime, DirectX::SimpleMath::Vector3 pos);

		ConstantBuffer<Sun::ShaderMatrix>* getLightMatrixBuffer() { return sun.getMatrixBuffer(); };
		ConstantBuffer<Sun::LightValues>*  getShaderBuffer() { return sun.getShaderBuffer(); };
		D3D11_VIEWPORT getViewPort() { return sun.getViewPort(); };
		Graphics::DepthStencil * getDepthStencil() { return &this->shadowDepthStencil; };
		ID3D11SamplerState ** getSampler() { return &this->shadowSampler; };

		void drawShadows(ID3D11DeviceContext * context, Graphics::Shader * shader);
		void clear(ID3D11DeviceContext * context);
	private:
		ModelInfo skySphere;

		Shader shader;
		ID3D11ShaderResourceView * srv;
		ID3D11ShaderResourceView * srv2;

		DepthStencil shadowDepthStencil;
		ID3D11SamplerState* shadowSampler;

		ConstantBuffer<DirectX::SimpleMath::Matrix> sphereTransformBuffer;

		Sun sun;

		void createSampler(ID3D11Device * device);

	};
}
