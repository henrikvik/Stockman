#pragma once
#include "RenderPass.h"
#include "../Fog.H"

namespace Graphics {

	class FogRenderPass : public RenderPass
	{
	public:
		FogRenderPass(
			std::initializer_list<ID3D11RenderTargetView*> targets,
			std::initializer_list<ID3D11ShaderResourceView*> resources = {},
			std::initializer_list<ID3D11Buffer*> buffers = {},
			ID3D11DepthStencilView * depthStencil = nullptr);

		// Inherited via RenderPass
		virtual void render() const override;
		virtual void update(float deltaTime) override;
		virtual wchar_t * name() const override;
	private:
		Fog fog;
	};
}