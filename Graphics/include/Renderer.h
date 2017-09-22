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

namespace Graphics
{
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

        ResourceManager resourceManager;
        D3D11_VIEWPORT viewPort;

        // Lånade Pekare
        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;

        // Egna Pekare
        ID3D11Buffer * instanceBuffer;		

        ///// SUPER TEMP
        ID3D11Buffer *GUIvb;
        ID3D11BlendState *transparencyBlendState;
		

		////LITE TEMP
		DirectionalLight lightDir;
		ID3D11DepthStencilView* shadowDSV;
		ID3D11ShaderResourceView* shadowSRV;
		ID3D11SamplerState* shadowSampler;


        void createInstanceBuffer();

        void cull();
        void writeInstanceData();
        void draw();
        void drawGUI();
		void drawShadows();
		

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

        void createBlendState();
        void createGUIBuffers();
		void createShadowMap();
    };
};