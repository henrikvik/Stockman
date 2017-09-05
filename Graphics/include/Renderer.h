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

namespace Graphics
{

	

    class Renderer
    {
    public:
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer);
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