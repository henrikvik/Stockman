#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "Camera.h"
#include "Datatypes.h"
#include "WICTextureLoader.h"
#include "Lights\LightGrid.h"
#include "Utility\DepthStencil.h"
#include "Utility\ConstantBuffer.h"
#include "Utility\StructuredBuffer.h"
#include "Utility\ShaderResource.h"
#include "Lights\Sun.h"
#include "HybrisLoader\HybrisLoader.h"
#include "Fog.h"
#include "Utility\PingPongBuffer.h"

#include <SpriteBatch.h>

#include "RenderPass\GUIRenderPass.h"


namespace Graphics
{
	class Renderer
    {
    public:

        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
        virtual ~Renderer();

        void render() const;
        void update(float deltaTime);
    private:

#pragma region Shared Shader Resources
        std::vector<RenderPass*> renderPasses;
        ShaderResource colorMap;
        ShaderResource glowMap;
        ShaderResource normalMap;
        DepthStencil shadowMap;
        DepthStencil depthStencil;

        ID3D11ShaderResourceView  * lightOpaqueGridSRV;
        ID3D11UnorderedAccessView * lightOpaqueGridUAV;
        StructuredBuffer<uint32_t> lightOpaqueIndexList;
        StructuredBuffer<Light>    lightsNew;

        ID3D11RenderTargetView * backBuffer;

        PingPongBuffer fakeBuffers;

#pragma region Instance Buffers
        using float4x4 = DirectX::SimpleMath::Matrix;
        using float4 = DirectX::SimpleMath::Vector4;
        using float3 = DirectX::SimpleMath::Vector3;
        using float2 = DirectX::SimpleMath::Vector2;

        struct InstanceStatic
        {
            float4x4 world;
            float4x4 worldInvT;
        };
        struct InstanceAnimated : InstanceStatic
        {
            float4x4 jointTransforms[20];
        };


        void writeInstanceBuffers();
        StructuredBuffer<InstanceStatic> instanceStaticBuffer;
        StructuredBuffer<InstanceAnimated> instanceAnimatedBuffer;
    #pragma endregion


    #pragma endregion
        Sun sun;

        D3D11_VIEWPORT viewPort;

		void clear() const;
		void swapBackBuffers();
		
#pragma region Foliage
		 
		ConstantBuffer <UINT> timeBuffer;
		UINT grassTime = 0;
		Shader foliageShader;
#pragma endregion


#pragma region RenderDebugInfo

        Shader debugRender;
        std::vector<RenderDebugInfo*> renderDebugQueue;
        StructuredBuffer<DirectX::SimpleMath::Vector3> debugPointsBuffer;
        ConstantBuffer<DirectX::SimpleMath::Color> debugColorBuffer;
        void renderDebugInfo(Camera* camera);

#pragma endregion
		Fog fog;

#pragma region Draw Functions and Buffers





#pragma endregion



    };
};