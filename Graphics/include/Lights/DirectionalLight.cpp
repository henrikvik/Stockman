#include "DirectionalLight.h"
using namespace DirectX::SimpleMath;

DirectionalLight::DirectionalLight(ID3D11Device * device, int width, int height, float drawDistance)
{
	bufferStruct.dir = Vector4(0, -1, 0, 0);
	bufferStruct.pos = Vector4(0, 10, 0, 0);

	projection = DirectX::XMMatrixOrthographicRH(float(width), float(height), 0.1, drawDistance);
	view = DirectX::XMMatrixLookToRH(bufferStruct.pos, bufferStruct.dir, Vector3(0, 1, 0));
	model = Matrix::CreateTranslation(bufferStruct.pos.x, bufferStruct.pos.y, bufferStruct.pos.z);

	bufferStruct.mvp = model * view * projection;


	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(ShaderValues);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &bufferStruct;

	device->CreateBuffer(&desc, &data, &this->buffer);
}

DirectionalLight::~DirectionalLight()
{
	SAFE_RELEASE(buffer);
}

void DirectionalLight::update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext * context)
{
}
