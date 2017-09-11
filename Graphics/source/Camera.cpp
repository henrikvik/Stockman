#include "Camera.h"
using namespace DirectX::SimpleMath;
using namespace Graphics;

Camera::Camera(ID3D11Device* device, int width, int height, float drawDistance, float fieldOfView)
{
	this->mFieldOfView = fieldOfView;
	this->mDrawDistance = drawDistance;

	this->mProjection = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, float(width)/height, 0.1f, drawDistance);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(Matrix);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	device->CreateBuffer(&desc, NULL, &this->mVPBuffer);

	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(Matrix);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	Matrix inv = mProjection.Invert();

	D3D11_SUBRESOURCE_DATA data = {0};
	data.pSysMem = &inv;

	device->CreateBuffer(&desc, &data, &this->mInvProjBuffer);
}

Camera::~Camera()
{
	this->mVPBuffer->Release();
	this->mInvProjBuffer->Release();
}

void Camera::setPos(DirectX::SimpleMath::Vector3 pos)
{
	this->mPos = pos;
}

void Camera::setForward(DirectX::SimpleMath::Vector3 forward)
{
	this->mForward = forward;
}

void Camera::setUp(DirectX::SimpleMath::Vector3 up)
{
	this->mUp = up;
}

void Camera::setRight(DirectX::SimpleMath::Vector3 right)
{
	this->mRight = right;
}

DirectX::SimpleMath::Vector3 Camera::getPos() const
{
	return this->mPos;
}

DirectX::SimpleMath::Vector3 Camera::getForward() const
{
	return this->mView.Forward();
}

DirectX::SimpleMath::Vector3 Camera::getUp() const
{
	return this->mView.Up();
}

DirectX::SimpleMath::Vector3 Camera::getRight() const
{
	return this->mView.Right();
}

ID3D11Buffer* Graphics::Camera::getBuffer()
{
	return this->mVPBuffer;
}

void Graphics::Camera::update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context)
{
	Matrix newView = DirectX::XMMatrixLookToLH(pos, forward, Vector3(0, 1, 0));

	if (newView != this->mView)
	{
		this->mView = newView;
		this->mPos = pos;

		Matrix vP = this->mProjection * this->mView;

		D3D11_MAPPED_SUBRESOURCE data;
		ZeroMemory(&data, sizeof(data));

		context->Map(this->mVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

		memcpy(data.pData, &vP, sizeof(Matrix));

		context->Unmap(this->mVPBuffer, 0);
	}
}
