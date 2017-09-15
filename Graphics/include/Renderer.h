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
#include <Resources\Shader.h>
#include <Datatypes.h>

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
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer);
		virtual ~Renderer();
        void render(Camera * camera);
        void queueRender(RenderInfo * renderInfo);
        void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);

    private:
        typedef  std::unordered_map<ModelID, std::vector<InstanceData>> InstanceQueue_t;

        // Lånade Pekare
        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;

        // Egna Pekare
        ID3D11DepthStencilView * dSV;
        ID3D11DepthStencilState * dSS;
        ID3D11Buffer * instanceBuffer;

		ID3D11BlendState *transparencyBlendState;

		//temp
		ID3D11ShaderResourceView* view;
		ID3D11Buffer * FSQuad2;
		ID3D11Buffer * defferedTestBuffer;
		ID3D11Buffer * instanceBuffer;
		ID3D11Buffer *GUIvb;
        GBuffer gbuffer;

        Shader simpleForward;
        ResourceManager resourceManager;
        D3D11_VIEWPORT viewPort;

        std::vector<RenderInfo*> renderQueue;
        InstanceQueue_t instanceQueue;

        void createGBuffer();
        void createInstanceBuffer();

        void cull();
        void draw();

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

		void drawDeffered();
		void createDepthStencil();
		void createCubeInstances();
		void drawGUI();
		void createBlendState();
		void createGUIBuffers();
		

        ///// SUPER TEMP
        TempCube cube;
    };

};