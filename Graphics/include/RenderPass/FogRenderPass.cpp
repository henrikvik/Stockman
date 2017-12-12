#include "FogRenderPass.h"
#include <Singletons\DebugWindow.h>
#include <Engine\Settings.h>


Graphics::FogRenderPass::FogRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, std::initializer_list<ID3D11ShaderResourceView*> resources, std::initializer_list<ID3D11Buffer*> buffers, ID3D11DepthStencilView * depthStencil)
	: RenderPass(targets, resources, buffers, depthStencil)
	, fog(Global::device)
{
	enabled = false;//Settings::getInstance().getFog();
	RegisterCommand("TOGGLE_FOG", 
	{
		enabled = !enabled;
		return "Fog Toggled";
	});
}

void Graphics::FogRenderPass::render() const
{
	if (enabled)
	{
		fog.renderFog(Global::context, targets[0], resources[0]);

	}
}

void Graphics::FogRenderPass::update(float deltaTime)
{
}

wchar_t * Graphics::FogRenderPass::name() const
{
	return nullptr;
}
