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
    Renderer::Renderer(
        ID3D11Device * device,
        ID3D11DeviceContext * deviceContext,
        ID3D11RenderTargetView * backBuffer, 
        Camera *camera
    )
        : depthStencil(device, WIN_WIDTH, WIN_HEIGHT)
        , fog(device)
        , foliageShader(device, SHADER_PATH("FoliageShader.hlsl"), VERTEX_DESC)
        , timeBuffer(device)
#pragma endregion
        , foliageInstanceBuffer(device, CpuAccess::Write, INSTANCE_CAP(FoliageRenderInfo))
        , staticInstanceBuffer(device, CpuAccess::Write, INSTANCE_CAP(StaticRenderInfo))
        , animatedInstanceBuffer(device, CpuAccess::Write, INSTANCE_CAP(AnimatedRenderInfo))
        , animatedJointsBuffer(device, CpuAccess::Write, INSTANCE_CAP(AnimatedRenderInfo))
        , fakeBuffers(WIN_WIDTH, WIN_HEIGHT)
#pragma region Shared Shader Resources
        , colorMap(WIN_WIDTH, WIN_HEIGHT)
        , glowMap(WIN_WIDTH, WIN_HEIGHT)
        , normalMap(WIN_WIDTH, WIN_HEIGHT)

        , shadowMap(Global::device, SHADOW_RESOLUTION, SHADOW_RESOLUTION)


        , lightOpaqueIndexList(CpuAccess::None, INDEX_LIST_SIZE)
        , lightsNew(CpuAccess::Write, INSTANCE_CAP(LightRenderInfo))
        , sun(shadowMap),
#pragma endregion
        DebugAnnotation(nullptr)


#pragma region CaNCeR!
    {
        grassTime = 0;
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
        HRESULT hr = Global::context->QueryInterface(IID_PPV_ARGS(&DebugAnnotation));
        if (!SUCCEEDED(hr)) {
            // failed to aquire debug annotation
        }

        this->backBuffer = backBuffer;

        viewPort = { 0 };
        viewPort.Width = WIN_WIDTH;
        viewPort.Height = WIN_HEIGHT;
        viewPort.MaxDepth = 1.0f;

        FXSystem = newd ParticleSystem(device, 4096, "Resources/Particles/base.part");

        TextureLoader::get().loadAll();

        renderPasses =
        {
            newd ParticleDepthRenderPass(depthStencil),
            newd DepthRenderPass(
                {},
                { staticInstanceBuffer, animatedInstanceBuffer, animatedJointsBuffer,
                foliageInstanceBuffer },
                {*Global::mainCamera->getBuffer(), grassTimeBuffer },
                depthStencil
            ),
            newd ShadowRenderPass(
                {}, 
                { staticInstanceBuffer, animatedInstanceBuffer },
                {*sun.getLightMatrixBuffer(), grassTimeBuffer },
                shadowMap
            ),
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
            newd SkyBoxRenderPass({ fakeBuffers }, {}, { *sun.getGlobalLightBuffer() }, depthStencil, &sun),
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
                    staticInstanceBuffer,
                    animatedInstanceBuffer,
                    animatedJointsBuffer,
                    foliageInstanceBuffer
                },
                {
                    *Global::mainCamera->getBuffer(),
                    *sun.getGlobalLightBuffer(),
                    *sun.getLightMatrixBuffer(),
                    grassTimeBuffer
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
            newd DepthOfFieldRenderPass(&fakeBuffers, {}, { depthStencil }), //this
            newd GlowRenderPass(&fakeBuffers, {}, { glowMap }), //and this
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
        SAFE_RELEASE(DebugAnnotation);

        for (auto & renderPass : renderPasses)
        {
            delete renderPass;
        }

        TextureLoader::get().unloadAll();
        ModelLoader::get().unloadAll();
    }

    void Renderer::update(float deltaTime)
    {
        grassTime += deltaTime;
        grassTimeBuffer.write(Global::context, &grassTime, sizeof(grassTime));
        
        /*
        static StaticRenderInfo infotest;
        infotest.model = Resources::Models::Staff;
        infotest.transform = DirectX::SimpleMath::Matrix::CreateTranslation({0, 10, 0});
        QueueRender(&infotest);
        */

        
        LightRenderInfo lightInfo;
        lightInfo.color = DirectX::Colors::WhiteSmoke;
        lightInfo.intensity = 0.3f;
        lightInfo.position = Global::mainCamera->getPos() + SimpleMath::Vector3(0, 0, 0);
        lightInfo.range = 10;
        QueueRender(lightInfo);

        /*QueueRender([](float dt) -> AnimatedRenderInfo
        {
            static float time = 0;
            AnimatedRenderInfo info;
            info.animationName = "Walk";
            info.animationTimeStamp = time;
            info.model = Resources::Models::SummonUnitWithAnim;
            info.transform = SimpleMath::Matrix::CreateTranslation(0, 1, -3) * SimpleMath::Matrix::CreateScale(5.0f);
            time += dt;
            if (time > 5) time = 0;
            return info;
        }(deltaTime));*/

		QueueRender([]()
		{
			StaticRenderInfo info;
			info.model = Resources::Models::UnitCube;
			info.transform = SimpleMath::Matrix::CreateTranslation(0, -1, 0) * SimpleMath::Matrix::CreateScale(1000, 1, 1000);
			info.useGridTexture = true;
			return info;
		}());

        QueueRender([]()
        {
            FoliageRenderInfo info;
            info.model = Resources::Models::Grass;
            info.transform = SimpleMath::Matrix::CreateTranslation(0, 0, -3);
            info.color = DirectX::SimpleMath::Vector3(1,1,1);
            info.useGridTexture = false;
            return info;
        }());

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
            if (DebugAnnotation) DebugAnnotation->BeginEvent(renderPass->name());
              renderPass->render();
            if (DebugAnnotation) DebugAnnotation->EndEvent();
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

    void Renderer::writeInstanceBuffers()
    {
        foliageInstanceBuffer.write([](StaticInstance * instanceBuffer)
        {
            for (auto & model_infos : RenderQueue::get().getQueue<FoliageRenderInfo>())
            {
                for (auto & sinfo : model_infos.second)
                {
                    StaticInstance instance = {};
                    instance.world = sinfo.transform;
                    instance.worldInvT = sinfo.transform.Invert().Transpose();
                    instance.color = sinfo.color;
                    instance.useGridTexture = sinfo.useGridTexture;
                    *instanceBuffer++ = instance;
                }
            }
        });

        staticInstanceBuffer.write([](StaticInstance * instanceBuffer)
        {
            auto & queue = RenderQueue::get().getQueue<StaticRenderInfo>();
            for (auto & model_infos : RenderQueue::get().getQueue<StaticRenderInfo>())
            {
                for (auto & sinfo : model_infos.second)
                {
                    StaticInstance instance = {};
                    instance.world = sinfo.transform;
                    instance.worldInvT = sinfo.transform.Invert().Transpose();
                    instance.color = sinfo.color;
                    instance.useGridTexture = sinfo.useGridTexture;
                    *instanceBuffer++ = instance;
                }
            }
        });

        {
            auto instanceBuffer = animatedInstanceBuffer.map();
            auto jointsBuffer   = animatedJointsBuffer.map();

            for (auto & model_infos : RenderQueue::get().getQueue<AnimatedRenderInfo>())
            {
                HybrisLoader::Skeleton * skeleton = &ModelLoader::get().getModel((Resources::Models::Files)model_infos.first)->getSkeleton();

                for (auto & info : model_infos.second)
                {
                    StaticInstance instance = {};
                    instance.world = info.transform;
                    instance.worldInvT = info.transform.Invert().Transpose();
                    instance.color = info.color;
                    instance.useGridTexture = info.useGridTexture;

                    AnimatedJoints joints;
                    if (strlen(info.animationName) != 0)
                    {
                        auto jointTransforms = skeleton->evalAnimation(info.animationName, info.animationTimeStamp);
                        for (size_t i = 0; i < jointTransforms.size(); i++)
                        {
                            joints.jointTransforms[i] = jointTransforms[i];
                        }
                    }

                    *instanceBuffer++ = instance;
                    *jointsBuffer++ = joints;
                }
            }

            animatedInstanceBuffer.unmap();
            animatedJointsBuffer.unmap();
        }

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
