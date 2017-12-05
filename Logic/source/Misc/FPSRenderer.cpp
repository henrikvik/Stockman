#include <Misc\FPSRenderer.h>
#include <Engine\DebugWindow.h>
#include <string>
#define FPS_STRING L"FPS: "

using namespace Logic;

static bool FPSGraph = false;

FPSRenderer::FPSRenderer()
{
    fps = 0;
    frames = 0;
    fpsTimer = 0.0f;
    renderInfo.color = DirectX::SimpleMath::Color{ 1, 1, 1 };
    renderInfo.font = Resources::Fonts::KG14;
    renderInfo.position = DirectX::SimpleMath::Vector2{ 5, 695 };
    renderInfo.text = L"Ta inte bort min kod - LW";
    FPSString = L" ";

    DebugWindow::getInstance()->registerCommand("FPS_GRAPH", [&](std::vector<std::string> &args)->std::string
    {
        FPSGraph = !FPSGraph;

        std::string response = "FPSGraph = ";
        response += FPSGraph ? "true" : "false";

        return response;
    });
}

FPSRenderer::~FPSRenderer()
{

}


void FPSRenderer::updateFPS(float deltaTime)
{
    static size_t avgframesCount = 250;
    static std::vector<float> fps(avgframesCount);
    static size_t i = 0;

    fps[i] = 1 / (deltaTime / 1000.f);
    i = ++i % avgframesCount;

    float avgFps = 0;
    float minFps = 1000000.f;
    float maxFps = 0.f;
    for (size_t i = 0; i < avgframesCount; i++)
    {
        avgFps += fps[i];
        minFps = min(minFps, fps[i]);
        maxFps = max(maxFps, fps[i]);
    }
    avgFps /= avgframesCount;

    int intFps = avgFps;
    FPSString = FPS_STRING + std::to_wstring(intFps);
    renderInfo.text = FPSString.c_str();

    if (FPSGraph) {
        char title[128];
        sprintf_s(title, "min: %d, max: %d, avg: %d", (int)minFps, (int)maxFps, (int)avgFps);
        ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.1f, 0.93f, 0.2f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.f, 0.f, 0.f, 0.f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, .25f, 0.f, .25f));
        ImGui::SetNextWindowPos(ImVec2(8, 720 - 200 - 120));

        if (ImGui::Begin("##test", &FPSGraph, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
            ImGui::PlotLines("##sdftest", fps.data(), fps.size(), 0, title, 0.f, 144.f, ImVec2(200, 120));
            ImGui::End();
        }

        ImGui::PopStyleColor(3);
    }
}

void Logic::FPSRenderer::render() const
{
    QueueRender(renderInfo);
}

TextRenderInfo Logic::FPSRenderer::getTextRenderInfo() const
{
    return this->renderInfo;
}
