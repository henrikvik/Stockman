#include "Renderer.h"
#include <stdio.h>
#include <Graphics\include\ThrowIfFailed.h>
#include <Engine\Constants.h>
#include "Utility\sizeofv.h"
#include <Keyboard.h>
#include <Engine\DebugWindow.h>

#include <Engine\Profiler.h>
#include "RenderQueue.h"

#include "Particles\ParticleSystem.h"
#include "Utility\TextureLoader.h"
#include "CommonStates.h"
#include "MainCamera.h"

#include "RenderPass\ForwardPlusRenderPass.h"
#include "RenderPass\DepthRenderPass.h"
#include "RenderPass\LightCullRenderPass.h"
#include "RenderPass\ShadowRenderPass.h"
#include "RenderPass\SkyBoxRenderPass.h"
#include "RenderPass\GlowRenderPass.h"
#include "RenderPass\ParticleRenderPass.h"
#include "RenderPass\SSAORenderPass.h"
#include "RenderPass\DepthOfFieldRenderPass.h"
#include "RenderPass\SnowRenderPass.h"
#include "RenderPass\BulletTimeRenderPass.h"
#include "RenderPass\DebugRenderPass.h"

#include "Utility\DebugDraw.h"


#define ANIMATION_HIJACK_RENDER false
#define USE_OLD_RENDER false

#define MAX_DEBUG_POINTS 10000
#define _INSTANCE_CAP 300

namespace Graphics
{
    uint32_t zero = 0;
    Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
        : depthStencil(device, WIN_WIDTH, WIN_HEIGHT)
#pragma region RenderDebugInfo
        , debugPointsBuffer(device, CpuAccess::Write, MAX_DEBUG_POINTS)
        , debugRender(device, SHADER_PATH("DebugRender.hlsl"))
        , debugColorBuffer(device)
#pragma endregion
        , fog(device)
        , foliageShader(device, SHADER_PATH("FoliageShader.hlsl"), VERTEX_DESC)
        , timeBuffer(device)


#pragma endregion
        , instanceStaticBuffer(device, CpuAccess::Write, INSTANCE_CAP(InstanceAnimated))
        , instanceAnimatedBuffer(device, CpuAccess::Write, INSTANCE_CAP(AnimatedRenderInfo))
        , fakeBuffers(WIN_WIDTH, WIN_HEIGHT)
#pragma region Shared Shader Resources
        , colorMap(WIN_WIDTH, WIN_HEIGHT)
        , glowMap(WIN_WIDTH, WIN_HEIGHT)
        , normalMap(WIN_WIDTH, WIN_HEIGHT)

        , shadowMap(Global::device, SHADOW_RESOLUTION, SHADOW_RESOLUTION)


        , lightOpaqueIndexList(CpuAccess::None, INDEX_LIST_SIZE)
        , lightsNew(CpuAccess::Write, INSTANCE_CAP(LightRenderInfo))
        , sun()
#pragma endregion


#pragma region CaNCeR!
    {
        Global::cStates = new CommonStates(device);
        Global::comparisonSampler = [&]() {
            ID3D11SamplerState * sampler = nullptr;
            D3D11_SAMPLER_DESC sDesc = {};
            sDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            sDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            sDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            sDesc.BorderColor[0] = 1;
            sDesc.BorderColor[1] = 1;
            sDesc.BorderColor[2] = 1;
            sDesc.BorderColor[3] = 1;
            sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
            sDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
            sDesc.MaxAnisotropy = 0;
            sDesc.MinLOD = 0;
            sDesc.MaxLOD = D3D11_FLOAT32_MAX;
            sDesc.MipLODBias = 0;

            ThrowIfFailed(device->CreateSamplerState(&sDesc, &sampler));
            return sampler;
        }();
        Global::mirrorSampler = [&]() {
            ID3D11SamplerState * sampler = nullptr;
            D3D11_SAMPLER_DESC sDesc = {};
            sDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            sDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            sDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
            sDesc.BorderColor[0] = 1;
            sDesc.BorderColor[1] = 1;
            sDesc.BorderColor[2] = 1;
            sDesc.BorderColor[3] = 1;
            sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
            sDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
            sDesc.MaxAnisotropy = 0;
            sDesc.MinLOD = 0;
            sDesc.MaxLOD = D3D11_FLOAT32_MAX;
            sDesc.MipLODBias = 0;

            ThrowIfFailed(device->CreateSamplerState(&sDesc, &sampler));
            return sampler;
        }();
        Global::transparencyBlendState = [&]() {
            ID3D11BlendState * state = nullptr;
            D3D11_BLEND_DESC blendDesc = { 0 };

            blendDesc.IndependentBlendEnable = false;
            blendDesc.AlphaToCoverageEnable = false;
            blendDesc.RenderTarget[0].BlendEnable = true;
            blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
            blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            //blendDesc.RenderTarget[0].LogicOp = D3D11_LOGIC_OP_CLEAR;

            ThrowIfFailed(device->CreateBlendState(&blendDesc, &state));
            return state;
        }();


        { // CaNCeR!
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Format = DXGI_FORMAT_R32G32_UINT;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
            desc.Width = (int)ceil(1280 / (float)BLOCK_SIZE);
            desc.Height = (int)ceil(720 / (float)BLOCK_SIZE);
            desc.SampleDesc.Count = 1;
            desc.MipLevels = 1;
            desc.ArraySize = 1;

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            ZeroMemory(&uavDesc, sizeof(uavDesc));
            uavDesc.Format = DXGI_FORMAT_R32G32_UINT;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_R32G32_UINT;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            ID3D11Texture2D *texture;
            ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
            ThrowIfFailed(device->CreateUnorderedAccessView(texture, &uavDesc, &lightOpaqueGridUAV));
            ThrowIfFailed(device->CreateShaderResourceView(texture, &srvDesc, &lightOpaqueGridSRV));
            SAFE_RELEASE(texture);
        }

#pragma endregion


        this->backBuffer = backBuffer;

        viewPort = { 0 };
        viewPort.Width = WIN_WIDTH;
        viewPort.Height = WIN_HEIGHT;
        viewPort.MaxDepth = 1.0f;

        FXSystem = newd ParticleSystem(device, 512, "Resources/Particles/base.part");

        TextureLoader::get().loadAll();

        renderPasses =
        {
            newd ParticleDepthRenderPass(depthStencil),
            newd DepthRenderPass({},  { instanceStaticBuffer }, {*Global::mainCamera->getBuffer()}, depthStencil),
            newd ShadowRenderPass({}, { instanceStaticBuffer }, {*sun.getLightMatrixBuffer()}, shadowMap),
            newd LightCullRenderPass(
                {},
                {
                    depthStencil,
                    lightsNew
                },
                {
                    *Global::mainCamera->getBuffer()
                },
                nullptr,
                {
                    lightOpaqueIndexList,
                    lightOpaqueGridUAV
                }
            ),
            newd SkyBoxRenderPass({ fakeBuffers }, {}, { *sun.getGlobalLightBuffer() }, depthStencil),
            newd ForwardPlusRenderPass(
                {
                    fakeBuffers,
                    glowMap,
                    normalMap
                },
                {
                    lightOpaqueIndexList,
                    lightOpaqueGridSRV,
                    lightsNew,
                    shadowMap,
                    instanceStaticBuffer
                },
                {
                    *Global::mainCamera->getBuffer(),
                    *sun.getGlobalLightBuffer()
                },
                depthStencil
            ),
            newd ParticleRenderPass(
                fakeBuffers,
                lightOpaqueGridSRV, 
                lightOpaqueIndexList, 
                lightsNew, 
                shadowMap, 
                *sun.getGlobalLightBuffer(),
                depthStencil
            ),
            newd SSAORenderPass(&fakeBuffers, {}, { depthStencil, normalMap }, {}, nullptr), //this
            //newd DepthOfFieldRenderPass(&fakeBuffers, {}, { depthStencil }), //this
            newd GlowRenderPass(&fakeBuffers, { }, { glowMap}), //and this
            newd SnowRenderPass(
                {
                    fakeBuffers
                },
                {
                    lightOpaqueIndexList,
                    lightOpaqueGridSRV,
                    lightsNew,
                    shadowMap
                },
                {
                    *sun.getLightMatrixBuffer(),
                    *sun.getGlobalLightBuffer()
                },
                depthStencil
            ),
            newd BulletTimeRenderPass(&fakeBuffers, {backBuffer}),
            newd DebugRenderPass({backBuffer},{},{*Global::mainCamera->getBuffer()}, depthStencil),
            newd GUIRenderPass({backBuffer}),
        };
    }


    Renderer::~Renderer()
    {
        delete FXSystem;

        delete Global::cStates;
        SAFE_RELEASE(Global::comparisonSampler);
        SAFE_RELEASE(Global::mirrorSampler);
        SAFE_RELEASE(Global::transparencyBlendState);
        SAFE_RELEASE(lightOpaqueGridUAV);
        SAFE_RELEASE(lightOpaqueGridSRV);

        for (auto & renderPass : renderPasses)
        {
            delete renderPass;
        }

        TextureLoader::get().unloadAll();
        ModelLoader::get().unloadAll();
    }

    void Renderer::update(float deltaTime)
    {
        /*
        static StaticRenderInfo infotest;
        infotest.model = Resources::Models::Staff;
        infotest.transform = DirectX::SimpleMath::Matrix::CreateTranslation({0, 10, 0});
        QueueRender(&infotest);
        */
        LightRenderInfo lightInfo;
        lightInfo.color = DirectX::Colors::DodgerBlue;
        lightInfo.intensity = 1;
        lightInfo.position = Global::mainCamera->getPos() + float3(0, 0, 4);
        lightInfo.range = 10;

        QueueRender(lightInfo);

        FXSystem->update(Global::context, Global::mainCamera, deltaTime);

        writeInstanceBuffers();
        sun.update();
   
        for (auto & renderPass : renderPasses)
        {
            PROFILE_BEGIN(__FUNCSIG__);
            renderPass->update(deltaTime);
            PROFILE_END();
        }

        fakeBuffers.reset();
    }

    void Renderer::render() const
    {
        clear();
        Global::context->RSSetViewports(1, &viewPort);

        for (auto & renderPass : renderPasses)
        {
            renderPass->render();
        }
    }

    void Renderer::clear() const
    {
        static float clearColor[4] = { 0 };
        Global::context->ClearRenderTargetView(backBuffer, clearColor);
        Global::context->ClearRenderTargetView(normalMap, clearColor);
        fakeBuffers.clear();
        Global::context->ClearRenderTargetView(glowMap, clearColor);
        Global::context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);
        Global::context->ClearDepthStencilView(shadowMap, D3D11_CLEAR_DEPTH, 1.f, 0);
    }

    void Renderer::renderDebugInfo(Camera* camera)
    {
        if (renderDebugQueue.size() == 0) return;

        Global::context->PSSetConstantBuffers(0, 1, *camera->getBuffer());
        Global::context->VSSetConstantBuffers(0, 1, *camera->getBuffer());

        Global::context->OMSetRenderTargets(1, &backBuffer, depthStencil);

        Global::context->VSSetShaderResources(0, 1, debugPointsBuffer);
        Global::context->PSSetConstantBuffers(1, 1, debugColorBuffer);

        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(debugRender, nullptr, 0);
        Global::context->PSSetShader(debugRender, nullptr, 0);


        for (RenderDebugInfo * info : renderDebugQueue)
        {
            debugPointsBuffer.write(
                Global::context,
                info->points->data(),
                (UINT)(info->points->size() * sizeof(DirectX::SimpleMath::Vector3))
            );

            debugColorBuffer.write(
                Global::context,
                &info->color,
                (UINT)sizeof(DirectX::SimpleMath::Color)
            );

            Global::context->IASetPrimitiveTopology(info->topology);
            Global::context->OMSetDepthStencilState(info->useDepth ? Global::cStates->DepthDefault() : Global::cStates->DepthNone(), 0);
            Global::context->Draw((UINT)info->points->size(), 0);
        }

        renderDebugQueue.clear();
    }

    void Renderer::writeInstanceBuffers()
    {
        instanceStaticBuffer.write([](InstanceStatic * instanceBuffer)
        {
            for (auto & model_infos : RenderQueue::get().getQueue<StaticRenderInfo>())
            {
                for (auto & sinfo : model_infos.second)
                {
                    InstanceStatic instance = {};
                    instance.world = sinfo.transform;
                    instance.worldInvT = sinfo.transform.Invert().Transpose();
                    *instanceBuffer++ = instance;
                }
            }
        });

        instanceAnimatedBuffer.write([](InstanceAnimated * instanceBuffer)
        {
            for (auto & model_infos : RenderQueue::get().getQueue<AnimatedRenderInfo>())
            {
                HybrisLoader::Skeleton * skeleton = &ModelLoader::get().getModel((Resources::Models::Files)model_infos.first)->getSkeleton();

                for (auto & info : model_infos.second)
                {
                    InstanceAnimated instance = {};
                    instance.world = info.transform;
                    instance.worldInvT = info.transform.Invert().Transpose();

                    if (strlen(info.animationName) != 0)
                    {
                        auto jointTransforms = skeleton->evalAnimation(info.animationName, info.animationTimeStamp);
                        for (size_t i = 0; i < jointTransforms.size(); i++)
                        {
                            instance.jointTransforms[i] = jointTransforms[i];
                        }
                    }

                    *instanceBuffer++ = instance;
                }
            }
        });

        lightsNew.write([](Light * lightBuffer)
        {
            for (auto & info : RenderQueue::get().getQueue<LightRenderInfo>())
            {
                Light light = {};
                light.range = info.range;
                light.intensity = info.intensity;
                light.color = DirectX::SimpleMath::Vector3(info.color.x, info.color.y, info.color.z);

                light.position = info.position;
                light.viewPosition = DirectX::SimpleMath::Vector4::Transform
                (
                    DirectX::SimpleMath::Vector4(info.position.x, info.position.y, info.position.z, 1.f), 
                    Global::mainCamera->getView()
                );

                Debug::PointLight(light);
                *lightBuffer++ = light;
            }

            for (size_t i = RenderQueue::get().getQueue<LightRenderInfo>().size(); i < INSTANCE_CAP(LightRenderInfo); i++)
            {
                Light light;
                ZeroMemory(&light, sizeof(light));
                *lightBuffer++ = light;
            }
        });
    }


}
