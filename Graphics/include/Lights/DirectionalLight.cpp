#include "DirectionalLight.h"
using namespace DirectX::SimpleMath;

DirectionalLight::DirectionalLight(ID3D11Device * device, int width, int height)
{
	shaderData.pos = Vector4(0, 10, 5, 1);

	projection = DirectX::XMMatrixOrthographicRH(100.f, 100.f, 1, 20);
	view = DirectX::XMMatrixLookAtRH(shaderData.pos, Vector3(0, 0, 0), Vector3(0, 1, 0));

	matrixData.vp = view * projection;


	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(ShaderMatrix);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &matrixData;

	device->CreateBuffer(&desc, &data, &this->matrixBuffer);

	desc.ByteWidth = sizeof(ShaderValues);
	data.pSysMem = &shaderData;

	device->CreateBuffer(&desc, &data, &this->shaderBuffer);


	viewPort = { 0 };
	viewPort.Height = height;
	viewPort.Width = width;
	viewPort.MaxDepth = 1.f;
}

DirectionalLight::~DirectionalLight()
{
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(shaderBuffer);
}

void DirectionalLight::update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext * context)
{
}
