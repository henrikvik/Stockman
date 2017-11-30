#include "SnowRenderPass.h"
#include "../Device.h"
#include "../MainCamera.h"
#include "../CommonState.h"
#include <SimpleMath.h>
#include <Engine\DebugWindow.h>

//temp
#include <Keyboard.h>

#define SNOW_RADIUS 65.f
#define MAX_SNOW 1024
#define PI 3.14159265f
#define ONE_DEG_IN_RAD 0.01745f
#define SNOW_SPEED 0.26f
#define WIND_CHANGE_TIME 5.5f

float getRandomFloat(float from, float to)
{
    // f is [0, 1]
    float f = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
    // return [from, to]
    return (f * std::abs(to - from)) + from;
}

using namespace DirectX::SimpleMath;

Graphics::SnowRenderPass::SnowRenderPass(
    std::initializer_list<ID3D11RenderTargetView*> targets, 
    std::initializer_list<ID3D11ShaderResourceView*> resources, 
    std::initializer_list<ID3D11Buffer*> buffers,
    ID3D11DepthStencilView * depthStencil):
    RenderPass(targets, resources, buffers, depthStencil), 
    snowBuffer(Global::device, CpuAccess::Write, MAX_SNOW),
    snowShader(Resources::Shaders::SnowShader, ShaderType::GS | ShaderType::VS | ShaderType::PS)
{
    snowFlakeCount = 0;

    DebugWindow::getInstance()->registerCommand("GFX_SET_SNOW", [&](std::vector<std::string> &args)->std::string
    {
        std::string catcher = "";
        try
        {
            if (args.size() != 0)
            {
                enabled = std::stoi(args[0]);

                if (enabled)
                    catcher = "Snow enabled!";

                else
                    catcher = "Snow disabled!";
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

void Graphics::SnowRenderPass::update(float deltaTime)
{
    //If the game restarts or something like that the snowflakes must be reset
    for (auto & info : RenderQueue::get().getQueue<SpecialEffectRenderInfo>())
    {
        if (info.type == info.Snow)
        {
            if (info.restart)
                initializeSnowflakes();
        }
    }

    static float windCounter = 0;
    static Vector3 randWindPrev(0, -SNOW_SPEED, 0);
    static Vector3 randWind(0, -SNOW_SPEED, 0);

    //temp
    static auto ks = DirectX::Keyboard::KeyboardStateTracker();
    ks.Update(DirectX::Keyboard::Get().GetState());

    if (ks.pressed.P)
    {
        initializeSnowflakes();
    }

    windCounter += deltaTime;
    if (WIND_CHANGE_TIME <= windCounter)
    {
        randWindPrev = randWind;
        windCounter = 0;
        randWind.x = getRandomFloat(-SNOW_SPEED, SNOW_SPEED);
        randWind.z = getRandomFloat(-SNOW_SPEED, SNOW_SPEED);
        randWind.y = -SNOW_SPEED;
    }



    for (int i = 0; i < snowFlakeCount; i++)
    {
        if ((snowFlakes[i].position - Global::mainCamera->getPos()).Length() > SNOW_RADIUS)
            moveSnowFlake(i);

        snowFlakes[i].distance = (snowFlakes[i].position - Global::mainCamera->getPos()).Length();
        snowFlakes[i].randomRot += getRandomFloat(0, ONE_DEG_IN_RAD * 5) * deltaTime * 62.5f; //62.5 * 0.016 = 1



        snowFlakes[i].position += Vector3::Lerp(randWindPrev, randWind, windCounter / WIND_CHANGE_TIME) * (deltaTime * 62.5);
    }

    if (snowFlakes.data() != NULL)
        snowBuffer.write(Global::context, snowFlakes.data(), snowFlakeCount * sizeof(SnowFlake));

}

void Graphics::SnowRenderPass::render() const
{
    if (snowFlakeCount > 0 && enabled)
    {
        Global::context->OMSetDepthStencilState(Global::cStates->DepthDefault(), 0);
        Global::context->RSSetState(Global::cStates->CullNone());

        Global::context->OMSetBlendState(Global::transparencyBlendState, 0, -1);

        Global::context->VSSetShaderResources(4, 1, snowBuffer);

        Global::context->GSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());
        Global::context->GSSetConstantBuffers(3, 1, &buffers[0]); //lightmatrixbuffer

        Global::context->PSSetConstantBuffers(1, 1, &buffers[1]); //lightDatabuffer

        Global::context->PSSetShaderResources(0, resources.size(), resources.data());
        
        Global::context->PSSetSamplers(1, 1, &Global::comparisonSampler);



        Global::context->OMSetRenderTargets(1, &targets[0], depthStencil); //depthmap & rendertarget


        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(snowShader, nullptr, 0);
        Global::context->PSSetShader(snowShader, nullptr, 0);
        Global::context->GSSetShader(snowShader, nullptr, 0);
        Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        Global::context->Draw(snowFlakeCount, 0);


        Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
        Global::context->PSSetShaderResources(1, 3, Global::nulls);
        Global::context->GSSetShader(nullptr, nullptr, 0);
    }
}

void Graphics::SnowRenderPass::addRandomSnowFlake()
{
    Vector3 randVec(getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS));

    Vector3 finalPos = Global::mainCamera->getPos() + randVec;

    SnowFlake flake;
    flake.position = finalPos;
    flake.randomRot = getRandomFloat(0, PI * 2.f);
    flake.distance = randVec.Length();

    snowFlakes.push_back(flake);

    snowFlakeCount++;
}

void Graphics::SnowRenderPass::moveSnowFlake(int snowFlake)
{
    Vector3 randVec(getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS));

    Vector3 finalPos = Global::mainCamera->getPos() + randVec;

    snowFlakes[snowFlake].position = finalPos;
    snowFlakes[snowFlake].randomRot = getRandomFloat(0, PI * 2.f);
    snowFlakes[snowFlake].distance = randVec.Length();
}

void Graphics::SnowRenderPass::clearSnow()
{
    snowFlakes.clear();
    snowFlakeCount = 0;
}

void Graphics::SnowRenderPass::initializeSnowflakes()
{
    clearSnow();

    for (int i = 0; i < MAX_SNOW; i++)
    {
        addRandomSnowFlake();
    }
}