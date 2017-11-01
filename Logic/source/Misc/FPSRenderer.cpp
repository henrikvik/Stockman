#include <Misc\FPSRenderer.h>
#define FPS_STRING L"FPS: "
using namespace Logic;

FPSRenderer::FPSRenderer()
{
	fps = 0;
	frames = 0;
	fpsTimer = 0.0f;

	fpsString.color = DirectX::SimpleMath::Color{ 1, 1, 1 };
	fpsString.font = Graphics::Font::SMALL;
	fpsString.pos = DirectX::SimpleMath::Vector2{ 5, 5 };
	fpsString.text = L"Ta inte bort min kod - LW";
}

FPSRenderer::~FPSRenderer()
{
}

void FPSRenderer::updateFPS(float deltaTime)
{
	fpsTimer += deltaTime;

	if (fpsTimer >= 998.f)
	{
		fps = frames;
		frames = 0;
		fpsTimer = 0;
		fpsString.text = FPS_STRING + std::to_wstring(fps);
	}
}

void FPSRenderer::renderFPS(Graphics::Renderer & renderer)
{
	frames++;

	renderer.queueText(&fpsString);
}