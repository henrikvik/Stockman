#include "SkyRenderer.h"
#include <Engine\Constants.h>
#include <DDSTextureLoader.h>
#include "ThrowIfFailed.h"

using namespace DirectX::SimpleMath;

SkyRenderer::SkyRenderer(ID3D11Device * device, int shadowRes) :
	shader(device, SHADER_PATH("SkyShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } }),
	cube(device),
	sun(device, shadowRes, shadowRes)
{
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, TEXTURE_PATH("skyBox.dds"), nullptr, &srv));
}

SkyRenderer::~SkyRenderer()
{
	SAFE_RELEASE(srv);
}

void SkyRenderer::renderSky(ID3D11DeviceContext * context, Graphics::Camera * cam)
{
	static UINT stride = sizeof(DirectX::SimpleMath::Vector3), offset = 0;

	context->IASetInputLayout(shader);
	context->IASetVertexBuffers(0, 1, &cube.vertexBuffer, &stride, &offset);
	context->VSSetShader(shader, nullptr, 0);
	context->PSSetShader(shader, nullptr, 0);
	context->PSSetShaderResources(0, 1, &srv);
	
	ID3D11Buffer * buffers[] =
	{
		cam->getBuffer(),
		cube.transformBuffer
	};

	context->VSSetConstantBuffers(0, 2, buffers);

	ID3D11Buffer * temp = sun.getShaderBuffer();

	context->PSSetConstantBuffers(2, 1, &temp);


	context->Draw(36, 0);
}

void SkyRenderer::update(ID3D11DeviceContext * context, float deltaTime, Vector3 pos)
{
	float radiansPerSecond = 0.01745 * deltaTime * 0.05f;

	sun.update(context, radiansPerSecond, pos);


	Matrix temp = Matrix::CreateTranslation(pos);

	D3D11_MAPPED_SUBRESOURCE data = {};

	context->Map(cube.transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	memcpy(data.pData, &temp, sizeof(Matrix));

	context->Unmap(cube.transformBuffer, 0);
}
