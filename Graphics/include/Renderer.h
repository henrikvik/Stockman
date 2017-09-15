#pragma once
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <unordered_map>
#include "Camera.h"
#include "Structs.h"
#include "Constants.h"
#include "WICTextureLoader.h"
#include <Resources\ResourceManager.h>
#include <SimpleMath.h>

namespace Graphics
{

	

    class Renderer
    {
    public:
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer);
		virtual ~Renderer();
        void render(Camera * camera);
        void queueRender(RenderInfo * renderInfo);

    private:
		//MYCKET TEMP
		struct TestCube
		{
			DirectX::SimpleMath::Vector3 pos;
			DirectX::SimpleMath::Vector2 uv;
			DirectX::SimpleMath::Vector3 normal;
			int mat;
		};

		ResourceManager resourceManager;

        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;
		ID3D11DepthStencilView * dSV;
		ID3D11DepthStencilState * dSS;

		ID3D11BlendState *transparencyBlendState;

		//temp
		ID3D11ShaderResourceView* view;
		ID3D11Buffer * FSQuad2;
		ID3D11Buffer * defferedTestBuffer;
		ID3D11Buffer * instanceBuffer;
		ID3D11Buffer *GUIvb;


        std::vector<RenderInfo*> renderQueue;
        typedef  std::unordered_map<int, std::vector<InstanceData>> InstanceQueue_t;
        InstanceQueue_t instanceQueue;
        GBuffer gbuffer;

        void createGBuffer();
        void cull();
        void draw();
		void drawDeffered();
		void createDepthStencil();
		void createCubeInstances();
		void drawGUI();
		void createBlendState();
		void createGUIBuffers();
		

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);
    };

};