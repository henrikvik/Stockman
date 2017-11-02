#include <Misc\FPSRenderer.h>
#include <Engine\DebugWindow.h>
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
    isActive = true;

    DebugWindow * debug = DebugWindow::getInstance();
    debug->registerCommand("LOG_TOGGLE_FPS", [&](std::vector<std::string> &args)->std::string
    {
        std::string catcher = "";
        try
        {
            if (args.size() != 0)
            {
                isActive = std::stoi(args[0]);

                if (isActive)
                    catcher = "FPS meter enabled!";

                else
                    catcher = "FPS meter disabled!";
            }
            else
            {
                catcher = "missing argument 0 or 1.";
            }
        }
        catch (const std::exception&)
        {
            catcher = "Argument must be 0 or 1.";
        }

        return catcher;
    });

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
    if (isActive)
	    renderer.queueText(&fpsString);
}