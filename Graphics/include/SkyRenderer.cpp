#include "SkyRenderer.h"
#include <Engine\Constants.h>
#include <DDSTextureLoader.h>
#include "ThrowIfFailed.h"

using namespace DirectX::SimpleMath;

SkyRenderer::SkyRenderer(ID3D11Device * device) :
	shader(device, SHADER_PATH("SkyShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } }),
	cube(device)
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
	
	Matrix temp = Matrix::CreateTranslation(cam->getPos());

	D3D11_MAPPED_SUBRESOURCE data = {};

	context->Map(cube.transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	
	memcpy(data.pData, &temp, sizeof(Matrix));

	context->Unmap(cube.transformBuffer, 0);
	
	ID3D11Buffer * buffers[] =
	{
		cam->getBuffer(),
		cube.transformBuffer
	};

	context->VSSetConstantBuffers(0, 2, buffers);


	context->Draw(36, 0);
}
