#include <Misc\FPSRenderer.h>
#include <string>
#define FPS_STRING L"FPS: "
using namespace Logic;

FPSRenderer::FPSRenderer()
{
	fps = 0;
	frames = 0;
	fpsTimer = 0.0f;

	fpsString.color = DirectX::SimpleMath::Color{ 1, 1, 1 };
	fpsString.font = Resources::Fonts::comicsans;
	fpsString.position = DirectX::SimpleMath::Vector2{ 5, 5 };
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
		fpsString.text = std::wstring(FPS_STRING + std::to_wstring(fps)).c_str();
	}
}

void FPSRenderer::render()
{
	frames++;
	RenderQueue::get().queue(&fpsString);
}