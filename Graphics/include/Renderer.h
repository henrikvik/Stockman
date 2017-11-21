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
        ID3DUserDefinedAnnotation *DebugAnnotation;

        #pragma region Shared Shader Resources
        std::vector<RenderPass*> renderPasses;
        ShaderResource colorMap;
        ShaderResource glowMap;
        ShaderResource normalMap;
        DepthStencil shadowMap;
        DepthStencil depthStencil;
        ShaderResource ssaoOutput;



        ID3D11ShaderResourceView *m_BloomSRV;
        std::vector<ID3D11ShaderResourceView *> m_BloomSRVMipChain;
        std::vector<ID3D11RenderTargetView *> m_BloomRTVMipChain;

        ID3D11ShaderResourceView  * lightOpaqueGridSRV;
        ID3D11UnorderedAccessView * lightOpaqueGridUAV;
        StructuredBuffer<uint32_t> lightOpaqueIndexList;
        StructuredBuffer<Light>    lightsNew;

        ID3D11ShaderResourceView *m_PositionSRV;
        ID3D11ShaderResourceView *m_AlbedoSpecularSRV;
        ID3D11ShaderResourceView *m_NormalSRV;

        ID3D11RenderTargetView *m_PositionRTV;
        ID3D11RenderTargetView *m_AlbedoSpecularRTV;
        ID3D11RenderTargetView *m_NormalRTV;

        ID3D11RenderTargetView * backBuffer;

        PingPongBuffer fakeBuffers;
        #pragma endregion

        #pragma region Instance Buffers
        void writeInstanceBuffers();

        struct StaticInstance
        {
            DirectX::SimpleMath::Matrix world;
            DirectX::SimpleMath::Matrix worldInvT;
            DirectX::SimpleMath::Vector3 color;
            UINT useGridTexture;
        };
        struct AnimatedJoints
        {
            DirectX::SimpleMath::Matrix jointTransforms[32];
        };

        float grassTime;
        ConstantBuffer<float> grassTimeBuffer;
        StructuredBuffer<StaticInstance> foliageInstanceBuffer;

        StructuredBuffer<StaticInstance> staticInstanceBuffer;
        StructuredBuffer<StaticInstance> animatedInstanceBuffer;
        StructuredBuffer<AnimatedJoints> animatedJointsBuffer;
        #pragma endregion

        Sun sun;

        D3D11_VIEWPORT viewPort;

		void clear() const;
		

		ConstantBuffer <UINT> timeBuffer;
		Shader foliageShader;
		Fog fog;
    };
};