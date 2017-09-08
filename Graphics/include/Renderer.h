#pragma once
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <unordered_map>
#include "Camera.h"
#include "Structs.h"
#include "Constants.h"
#include "ShaderHandler.h"
#include "WICTextureLoader.h"
#include "CommonStates.h"

namespace Graphics
{
    class Renderer
    {
    public:
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
		virtual ~Renderer();
        void render(Camera * camera);
        void qeueuRender(RenderInfo * renderInfo);
		//TODO: RELESASEA ALLA SAKER
    private:
		//MYCKET TEMP
		struct TestQuad
		{
			DirectX::SimpleMath::Vector3 pos;
			DirectX::SimpleMath::Vector2 uv;
			DirectX::SimpleMath::Vector3 normal;
			int mat;
		};

		DirectX::CommonStates *states;

		void createLightGrid(Camera *camera);
		void cullLightGrid(Camera *camera);

		DispatchParams gridParams;
		ID3D11Buffer *gridParamsBuffer;
		ID3D11Buffer *gridFrustrums;

		// TEMP {

		ID3D11DepthStencilView *depthDSV;
		ID3D11ShaderResourceView *depthSRV;
		ID3D11Buffer *planeBuffer;
		int planeVS;
		int planePS;

		void drawForward(Camera *camera);

		// TEMP }

		ID3D11UnorderedAccessView *gridDebugUAV;
		ID3D11ShaderResourceView *gridDebugSRV;

		ID3D11Buffer *gridResetIndexCounterBuffer;
		ID3D11UnorderedAccessView *gridResetIndexCounterUAV;
		ID3D11ShaderResourceView *gridResetIndexCounterSRV;

		ID3D11Buffer *gridOpaqueIndexCounterBuffer;
		ID3D11UnorderedAccessView *gridOpaqueIndexCounterUAV;
		ID3D11ShaderResourceView *gridOpaqueIndexCounterSRV;

		ID3D11Buffer *gridTransparentIndexCounterBuffer;
		ID3D11UnorderedAccessView *gridTransparentIndexCounterUAV;
		ID3D11ShaderResourceView *gridTransparentIndexCounterSRV;

		ID3D11UnorderedAccessView *gridOpaqueIndexListUAV;
		ID3D11ShaderResourceView *gridOpaqueIndexListSRV;
		ID3D11UnorderedAccessView *gridTransparentIndexListUAV;
		ID3D11ShaderResourceView *gridTransparentIndexListSRV;

		ID3D11UnorderedAccessView *gridOpaqueLightGridUAV;
		ID3D11ShaderResourceView *gridOpaqueLightGridSRV;
		ID3D11UnorderedAccessView *gridTransparentLightGridUAV;
		ID3D11ShaderResourceView *gridTransparentLightGridSRV;
		
		ID3D11Buffer *gridLights;
		ID3D11UnorderedAccessView *gridLightsUAV;
		ID3D11ShaderResourceView *gridLightsSRV;

		ID3D11UnorderedAccessView *gridFrustrumsUAV;
		ID3D11ShaderResourceView *gridFrustrumsSRV;
		ID3D11ShaderResourceView *gradientSRV;
		int gridFrustumGenerationCS;
		int gridCullingCS;

        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;
		ShaderHandler shaderHandler;

		//temp
		ID3D11ShaderResourceView* view;
		ID3D11Buffer * FSQuad2;
		ID3D11Buffer * defferedTestBuffer;

		int shaders[2];
		int defferedShaders[2];

        std::vector<RenderInfo*> renderQueue;
        typedef  std::unordered_map<int, std::vector<InstanceData>> InstanceQueue_t;
        InstanceQueue_t instanceQueue;
        GBuffer gbuffer;

        ID3D11Buffer *instanceBuffer;

        void createGBuffer();
        void cull();
        void draw();
		void drawDeffered();

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);
    };

};