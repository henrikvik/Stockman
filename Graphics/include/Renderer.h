#pragma once
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
#include "LightGrid.h"

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
                {{-1, -1, -1},{ 0,  0, -1},{ 0, 0},{ 0,0},{ 0,0}},
                {{-1,  1, -1},{ 0,  0, -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1},{ 0,  0, -1},{ 1, 0},{ 0,0},{ 0,0}},

                {{ 1, -1, -1},{ 0,  0, -1},{ 1, 0},{ 0,0},{ 0,0}},
                {{-1,  1, -1},{ 0,  0, -1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1,  1, -1},{ 0,  0, -1},{ 1, 1},{ 0,0},{ 0,0}},

                // TOP
                {{-1,  1, -1},{ 0,  1,  0},{ 0, 0},{ 0,0},{ 0,0}},
                {{-1,  1,  1},{ 0,  1,  0},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1,  1, -1},{ 0,  1,  0},{ 1, 0},{ 0,0},{ 0,0}},

                {{ 1,  1, -1},{ 0,  1,  0},{ 1, 0},{ 0,0},{ 0,0}},
                {{-1,  1,  1},{ 0,  1,  0},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1,  1,  1},{ 0,  1,  0},{ 1, 1},{ 0,0},{ 0,0}},

                // LEFT
                {{-1, -1,  1},{-1,  0,  0},{ 0, 1},{ 0,0},{ 0,0}},
                {{-1,  1,  1},{-1,  0,  0},{ 1, 1},{ 0,0},{ 0,0}},
                {{-1, -1, -1},{-1,  0,  0},{ 0, 0},{ 0,0},{ 0,0}},

                {{-1, -1, -1},{-1,  0,  0},{ 0, 0},{ 0,0},{ 0,0}},
                {{-1,  1,  1},{-1,  0,  0},{ 1, 1},{ 0,0},{ 0,0}},
                {{-1,  1, -1},{-1,  0,  0},{ 1, 0},{ 0,0},{ 0,0}},

                // RIGHT
                {{ 1,  1,  1},{ 1,  0,  0},{ 1, 1},{ 0,0},{ 0,0}},
                {{ 1, -1,  1},{ 1,  0,  0},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1},{ 1,  0,  0},{ 0, 0},{ 0,0},{ 0,0}},

                {{ 1,  1,  1},{ 1,  0,  0},{ 1, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1},{ 1,  0,  0},{ 0, 0},{ 0,0},{ 0,0}},
                {{ 1,  1, -1},{ 1,  0,  0},{ 1, 0},{ 0,0},{ 0,0}},

                // BACK
                {{-1,  1,  1},{ 0,  0,  1},{ 0, 1},{ 0,0},{ 0,0}},
                {{-1, -1,  1},{ 0,  0,  1},{ 0, 0},{ 0,0},{ 0,0}},
                {{ 1, -1,  1},{ 0,  0,  1},{ 1, 0},{ 0,0},{ 0,0}},

                {{-1,  1,  1},{ 0,  0,  1},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1,  1},{ 0,  0,  1},{ 1, 0},{ 0,0},{ 0,0}},
                {{ 1,  1,  1},{ 0,  0,  1},{ 1, 1},{ 0,0},{ 0,0}},

                // BOTTOM
                {{-1, -1,  1},{ 0, -1,  0},{ 0, 1},{ 0,0},{ 0,0}},
                {{-1, -1, -1},{ 0, -1,  0},{ 0, 0},{ 0,0},{ 0,0}},
                {{ 1, -1, -1},{ 0, -1,  0},{ 1, 0},{ 0,0},{ 0,0}},

                {{-1, -1,  1},{ 0, -1,  0},{ 0, 1},{ 0,0},{ 0,0}},
                {{ 1, -1, -1},{ 0, -1,  0},{ 1, 0},{ 0,0},{ 0,0}},
                {{ 1, -1,  1},{ 0, -1,  0},{ 1, 1},{ 0,0},{ 0,0}}
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
        GBuffer gbuffer;

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
		ID3D11DepthStencilView * dSV;
		ID3D11ShaderResourceView* depthSRV;

		ID3D11DepthStencilState * dSS;

        // Egna Pekare
        ID3D11Buffer * instanceBuffer;		

        ///// SUPER TEMP
        TempCube cube;
        ID3D11Buffer *GUIvb;
        ID3D11BlendState *transparencyBlendState;

        void createGBuffer();
        void createInstanceBuffer();

        void cull();
        void writeInstanceData();
        void draw();
        void drawGUI();
		void createDepthStencil();
		

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

        void createBlendState();
        void createGUIBuffers();
    };
};