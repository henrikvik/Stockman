#include "Sun.h"
#include <algorithm>
#include "../Device.h"
#include "../MainCamera.h"
#include <Singletons\DebugWindow.h>
#define PI 3.14159265f
#define ONE_DEG_IN_RAD 0.01745f
#define SUNSET_TIME 0.5f
#define DAY_NIGHT_ON false
#include <Singletons\DebugWindow.h>

static bool DebugShadowConstants = false;
namespace Graphics
{
    Sun::Sun(ID3D11ShaderResourceView *shadowMap)
        :
            lightMatrixBuffer(Global::device),
            globalLightBuffer(Global::device),
            m_ShadowMap(shadowMap)
    {
        pos = DirectX::SimpleMath::Vector4(0, 50, 0.5, 1);

        projection = DirectX::XMMatrixOrthographicRH(200.f, 200.f, 20, 200);
        view = DirectX::XMMatrixLookAtRH(pos, DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 1, 0));

        matrixData.vp = view * projection;

        globalLight.color = DirectX::SimpleMath::Vector3(0.1, 0.1, 0.3);
        globalLight.ambient = DirectX::SimpleMath::Vector3(0.2, 0.2, 0.2);
        DebugWindow::getInstance()->registerCommand("GFX_DEBUG_SHADOWS", [&](std::vector<std::string> &args)->std::string
        {
            DebugShadowConstants = !DebugShadowConstants;

            std::string response = "DebugShadowConstants = ";
            response += DebugShadowConstants ? "true" : "false";

            return response;
        });
    }

    Sun::~Sun()
    {
    }

    void Sun::update()
    {
        static DirectX::SimpleMath::Vector4 basepos(45.f, 14.f, 43.f, 1.f);
        static DirectX::SimpleMath::Vector3 lightdir(0.310f, 0.906f, 0.288f);
        static DirectX::SimpleMath::Vector3 lightcolor(0.1, 0.1, 0.3);
        static DirectX::SimpleMath::Vector3 ambient(0.2, 0.2, 0.2);
        static float nearPlane = 9.f;
        static float farPlane = 201.f;
        static float distance = 84.1f;
        static float width = 437.f;
        if (DebugShadowConstants) {
            ImGui::Begin("Shadow Constants");

            ImGui::DragFloat3("Position", (float*)&basepos, -1.f, 1.f);
            ImGui::SliderFloat3("Direction", (float*)&lightdir, 0.1, 1.f);
            ImGui::ColorEdit3("Color", (float*)&lightcolor);
            ImGui::ColorEdit3("Ambient", (float*)&ambient);
            ImGui::DragFloatRange2("Near/Far", &nearPlane, &farPlane, 1.f, 0.f, 400.f);
            ImGui::DragFloat("Distance", &distance, 1.f, 0.1f, 250.f);
            ImGui::DragFloat("Width", &width, 1.f, 0.1f, 500.f);
            auto w = ImGui::GetContentRegionAvailWidth();
            ImGui::Image(m_ShadowMap, ImVec2(w, w));

            ImGui::End();
        }

        lightdir.Normalize();
        m_LightDir = lightdir;

        DirectX::SimpleMath::Vector3 pos = lightdir * distance;

        auto proj = DirectX::XMMatrixOrthographicRH(width, width, nearPlane, farPlane);
        auto view = DirectX::XMMatrixLookAtRH(basepos + pos, DirectX::SimpleMath::Vector3(basepos) + pos - lightdir, DirectX::SimpleMath::Vector3(0, 1, 0));

        //a little bit temp, might be final totally final now
        static float rotationDeg = rotationDeg = PI * 0.25f;

        DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationZ(rotationDeg);

        this->globalLight.position = basepos + pos; // DirectX::SimpleMath::Vector4::Transform(pos, rotation);
        this->globalLight.color = lightcolor;
        this->globalLight.ambient = ambient;
        //this->globalLight.position = globalLight.position + Global::mainCamera->getPos();
        //view = DirectX::XMMatrixLookAtRH(globalLight.position, Global::mainCamera->getPos(), DirectX::SimpleMath::Vector3(0, 1, 0));
        matrixData.vp = view * proj;

        globalLightBuffer.write(Global::context, &globalLight, sizeof(globalLight));
        lightMatrixBuffer.write(Global::context, &matrixData, sizeof(matrixData));
    }

    float Sun::snap(float value, float minVal, float maxVal)
    {
        value = max(min(value, maxVal), minVal);
        return value;
    }
}