#include <Resources/ResourceManager.h>
#include <Engine\Constants.h>

namespace Graphics
{
    ResourceManager::ResourceManager()
    {

    }

    ResourceManager::~ResourceManager()
    {
        for (int i = 0; i < NROFSAMPLERS; i++)
        {
            this->sampleStates[i]->Release();
        }
    }

	void ResourceManager::initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
	{
		meshManager.initialize(gDevice, gDeviceContext);
		materialManager.initialize(gDevice, gDeviceContext);
		brfImporterHandler.initialize(meshManager, materialManager);

		brfImporterHandler.loadFile(MODEL_PATH_STR("kub.brf"), true, true, false, false);
		brfImporterHandler.loadFile(MODEL_PATH_STR("kub2.brf"), true, true, false, false);

		initShaders(gDevice);

    }

    ModelInfo ResourceManager::getModelInfo(ModelID modelID)
    {
        Mesh & mesh = meshManager.GetMeshes()->at(modelID);

        ModelInfo info = {
            mesh.GetIndexCount(),
            mesh.getIndexBuffer(),
            mesh.getVertexBuffer()
        };

        return info;
    }

    void ResourceManager::initShaders(ID3D11Device * gDevice)
    {

        D3D11_INPUT_ELEMENT_DESC desc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        shaderHandler.createVertexShader(gDevice, SHADER_PATH("FullscreenQuad.hlsl"), "VS", desc, ARRAYSIZE(desc));
        shaderHandler.createPixelhader(gDevice, SHADER_PATH("FullscreenQuad.hlsl"), "PS");

		D3D11_INPUT_ELEMENT_DESC descDeffered[] = VERTEX_INSTANCE_DESC;

		shaderHandler.createVertexShader(gDevice, SHADER_PATH("Forward.hlsl"), "VS", descDeffered, ARRAYSIZE(descDeffered));
		shaderHandler.createPixelhader(gDevice, SHADER_PATH("Forward.hlsl"), "PS");


		shaderHandler.createComputeShader(gDevice, SHADER_PATH("LightGridGeneration.hlsl"), "CS");
		shaderHandler.createComputeShader(gDevice, SHADER_PATH("LightGridCulling.hlsl"), "CS");


		createSamplerState(gDevice);

		D3D11_INPUT_ELEMENT_DESC GUIdesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "ELEMENT", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		shaderHandler.createVertexShader(gDevice, SHADER_PATH("GUIShader.hlsl"), "VS", GUIdesc, ARRAYSIZE(GUIdesc));
		shaderHandler.createPixelhader(gDevice, SHADER_PATH("GUISHADER.hlsl"), "VS");
	}

    void ResourceManager::setShaders(VertexShaderID vertexID, GeometryShaderID geometryID, PixelShaderID pixelID, ID3D11DeviceContext* context)
    {
        this->shaderHandler.setShaders(vertexID, geometryID, pixelID, context);
    }

    void ResourceManager::setShaders(VertexShaderID vertexID, PixelShaderID pixelID, ID3D11DeviceContext * context)
    {
        this->shaderHandler.setShaders(vertexID, NO_SHADER, pixelID, context);
    }

	//TODO: EVERYTHING
	void ResourceManager::setComputeShader(ComputeShaderID id, ID3D11DeviceContext * context)
	{
		this->shaderHandler.setComputeShader(id, context);
	}

    void ResourceManager::setSampler(SamplerID id, ID3D11DeviceContext * context)
    {
        context->PSSetSamplers(0, 1, &sampleStates[id]);
    }

    void ResourceManager::createSamplerState(ID3D11Device * device)
    {
        D3D11_SAMPLER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.BorderColor[0] = 1;
        desc.BorderColor[1] = 1;
        desc.BorderColor[2] = 1;
        desc.BorderColor[3] = 1;

        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.MaxAnisotropy = 1;

        device->CreateSamplerState(&desc, &this->sampleStates[SamplerID::pointSampler]);
    }


    void ResourceManager::release()
    {
        brfImporterHandler.release();
        meshManager.release();
    }
}