#include <Misc\FPSRenderer.h>
#include <string>
#define FPS_STRING L"FPS: "

using namespace Logic;

FPSRenderer::FPSRenderer()
{
	fps = 0;
	frames = 0;
	fpsTimer = 0.0f;

	renderInfo.color = DirectX::SimpleMath::Color{ 1, 1, 1 };
	renderInfo.font = Resources::Fonts::KG14;
	renderInfo.position = DirectX::SimpleMath::Vector2{ 5, 5 };
	renderInfo.text = L"Ta inte bort min kod - LW";
}

FPSRenderer::~FPSRenderer()
{
}

void FPSRenderer::updateFPS(float deltaTime)
{
    static size_t avgframesCount = 10;
    static std::vector<float> fps(avgframesCount);
    static size_t i = 0;

    fps[i] = (1 / (deltaTime / 1000.f));
    i = ++i % avgframesCount;

    float avgFps = 0;    
    for (size_t i = 0; i < avgframesCount; i++)
    {
        avgFps += fps[i];
    }
    avgFps /= avgframesCount;

    std::wstring FPSString = FPS_STRING + std::to_wstring(avgFps);
    renderInfo.text = FPSString.c_str();
    //renderInfo.text = L"cunt ass";

}

void Logic::FPSRenderer::render() const
{
    RenderQueue::get().queue(&renderInfo);
}
