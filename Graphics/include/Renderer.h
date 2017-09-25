#pragma once
#include "Resources\ResourceManager.h"
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <unordered_map>
#include "Camera.h"
#include "Structs.h"
#include "WICTextureLoader.h"
#include "Resources\ResourceManager.h"
#include <SimpleMath.h>
#include "Resources\Shader.h"
#include "Datatypes.h"
#include "Lights\LightGrid.h"
#include "Resources\DepthStencil.h"
#include "Lights\DirectionalLight.h"
#include <SpriteBatch.h>

namespace Graphics
{


    struct  TempCube
    {
        ID3D11Buffer * vertexBuffer;
        TempCube(ID3D11Device * device)
        {
            Vertex vertices[] =
            {
                // FRONT
                {{-1, -1, -1 * -1},{ 0,  0, -1 * -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{-1,  1, -1 * -1},{ 0,  0, -1 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1 * -1},{ 0,  0, -1 * -1},{ 1, 0},{ 0,0},{ 0,0}},
							
                {{ 1, -1, -1 * -1},{ 0,  0, -1 * -1},{ 1, 0},{ 0,0},{ 0,0}},
                {{-1,  1, -1 * -1},{ 0,  0, -1 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1,  1, -1 * -1},{ 0,  0, -1 * -1},{ 1, 1},{ 0,0},{ 0,0}},
							
                // TOP		
                {{-1,  1, -1 * -1},{ 0,  1,  0 * -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{-1,  1,  1 * -1},{ 0,  1,  0 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1,  1, -1 * -1},{ 0,  1,  0 * -1},{ 1, 0},{ 0,0},{ 0,0}},
							
                {{ 1,  1, -1 * -1},{ 0,  1,  0 * -1},{ 1, 0},{ 0,0},{ 0,0}},
                {{-1,  1,  1 * -1},{ 0,  1,  0 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1,  1,  1 * -1},{ 0,  1,  0 * -1},{ 1, 1},{ 0,0},{ 0,0}},
					
                // LEFT		
                {{-1, -1,  1 * -1},{-1,  0,  0 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{-1,  1,  1 * -1},{-1,  0,  0 * -1},{ 1, 1},{ 0,0},{ 0,0}},
                {{-1, -1, -1 * -1},{-1,  0,  0 * -1},{ 0, 0},{ 0,0},{ 0,0}},
							 
                {{-1, -1, -1 * -1},{-1,  0,  0 * -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{-1,  1,  1 * -1},{-1,  0,  0 * -1},{ 1, 1},{ 0,0},{ 0,0}},
                {{-1,  1, -1 * -1},{-1,  0,  0 * -1},{ 1, 0},{ 0,0},{ 0,0}},
							
                // RIGHT	
                {{ 1,  1,  1 * -1},{ 1,  0,  0 * -1},{ 1, 1},{ 0,0},{ 0,0}},
                {{ 1, -1,  1 * -1},{ 1,  0,  0 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1 * -1},{ 1,  0,  0 * -1},{ 0, 0},{ 0,0},{ 0,0}},
							
                {{ 1,  1,  1 * -1},{ 1,  0,  0 * -1},{ 1, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1 * -1},{ 1,  0,  0 * -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{ 1,  1, -1 * -1},{ 1,  0,  0 * -1},{ 1, 0},{ 0,0},{ 0,0}},
							
                // BACK		
                {{-1,  1,  1 * -1},{ 0,  0,  1 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{-1, -1,  1 * -1},{ 0,  0,  1 * -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{ 1, -1,  1 * -1},{ 0,  0,  1 * -1},{ 1, 0},{ 0,0},{ 0,0}},
							
                {{-1,  1,  1 * -1},{ 0,  0,  1 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1,  1 * -1},{ 0,  0,  1 * -1},{ 1, 0},{ 0,0},{ 0,0}},
                {{ 1,  1,  1 * -1},{ 0,  0,  1 * -1},{ 1, 1},{ 0,0},{ 0,0}},
							
                // BOTTOM	
                {{-1, -1,  1 * -1},{ 0, -1,  0 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{-1, -1, -1 * -1},{ 0, -1,  0 * -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{ 1, -1, -1 * -1},{ 0, -1,  0 * -1},{ 1, 0},{ 0,0},{ 0,0}},
							 
                {{-1, -1,  1 * -1},{ 0, -1,  0 * -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1 * -1},{ 0, -1,  0 * -1},{ 1, 0},{ 0,0},{ 0,0}},
                {{ 1, -1,  1 * -1},{ 0, -1,  0 * -1},{ 1, 1},{ 0,0},{ 0,0}}
            };

            D3D11_SUBRESOURCE_DATA subData = {};
            subData.pSysMem = vertices;

            D3D11_BUFFER_DESC desc = { 0 };
            desc.ByteWidth = sizeof(vertices);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            device->CreateBuffer(&desc, &subData, &vertexBuffer);
        }

        ~TempCube() 
        { 
            vertexBuffer->Release(); 
        }
    };


    class Renderer
    {
    public:
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
		virtual ~Renderer();
        void render(Camera * camera);
        void queueRender(RenderInfo * renderInfo);
        void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);

    private:
        typedef  std::unordered_map<ModelID, std::vector<InstanceData>> InstanceQueue_t;
        std::vector<RenderInfo*> renderQueue;
        InstanceQueue_t instanceQueue;

        DepthStencil depthStencil;

		LightGrid grid;
		DirectX::CommonStates *states;

        Shader fullscreenQuad;
        Shader simpleForward;
        Shader forwardPlus;
        ComputeShader lightGridCull;
        //ComputeShader lightGridGen; 

        ResourceManager resourceManager;
        D3D11_VIEWPORT viewPort;

        // Lånade Pekare
        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;

        // Egna Pekare
        ID3D11Buffer * instanceBuffer;		

        ///// SUPER TEMP
        TempCube cube;
        ID3D11Buffer *GUIvb;
        ID3D11BlendState *transparencyBlendState;
		

		////LITE TEMP
		DirectionalLight lightDir;
		ID3D11DepthStencilView* shadowDSV;
		ID3D11ShaderResourceView* shadowSRV;
		ID3D11SamplerState* shadowSampler;

        std::unique_ptr<DirectX::SpriteBatch> menuSprite;


        void createInstanceBuffer();

        void cull();
        void writeInstanceData();
        void draw();
        void drawGUI();
		void drawShadows();


        void drawMenu(Graphics::MenuInfo *info);
		

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

        void createBlendState();
        void createGUIBuffers();
		void createShadowMap();
    };
};