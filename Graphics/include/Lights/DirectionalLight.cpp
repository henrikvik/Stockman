#include "DirectionalLight.h"
using namespace DirectX::SimpleMath;

DirectionalLight::DirectionalLight(ID3D11Device * device, int width, int height)
{
	pos = Vector4(0, 50, 0.5, 1);

	projection = DirectX::XMMatrixOrthographicRH(100.f, 100.f, 1, 100);
	view = DirectX::XMMatrixLookAtRH(pos, Vector3(0, 0, 0), Vector3(0, 1, 0));

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

void DirectionalLight::update(ID3D11DeviceContext * context, Vector3 offset)
{
	//a little bit temp, might be final
	static float rotationDeg = 0;

	//Enable to get the day noght solko
	//rotationDeg += 0.01745 * 0.25;
	rotationDeg = 3.14 * 0.25;

	if (rotationDeg >= 3.14159265 * 2)
		rotationDeg = 0;

	Matrix rotation = Matrix::CreateRotationZ(rotationDeg);

	if (rotationDeg > 3.14 * 0.5 && rotationDeg < 3.14 * 1.5)
	{
		rotation = Matrix::CreateRotationZ(3.14 * 0.5);
	}

	this->shaderData.pos = Vector4::Transform(pos, rotation);
	this->shaderData.pos = shaderData.pos + offset;
	
	
	view = DirectX::XMMatrixLookAtRH(shaderData.pos, offset, Vector3(0, 1, 0));
	matrixData.vp = view * projection;


	D3D11_MAPPED_SUBRESOURCE data = {};
	
	context->Map(shaderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	memcpy(data.pData, &shaderData, sizeof(shaderData));

	context->Unmap(shaderBuffer, 0);

	context->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	memcpy(data.pData, &matrixData, sizeof(matrixData));

	context->Unmap(matrixBuffer, 0);

}
