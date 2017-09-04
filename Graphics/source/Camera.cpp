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
	desc.ByteWidth = sizeof(Matrix) * 2;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	device->CreateBuffer(&desc, NULL, &this->mVPBuffer);
}

Camera::~Camera()
{
	this->mVPBuffer->Release();
}

void Camera::setPos(DirectX::SimpleMath::Vector3 pos)
{
}

void Camera::setForward(DirectX::SimpleMath::Vector3 forward)
{
}

void Camera::setUp(DirectX::SimpleMath::Vector3 up)
{
}

void Camera::setRight(DirectX::SimpleMath::Vector3 right)
{
}

DirectX::SimpleMath::Vector3 Camera::getPos() const
{
	return DirectX::SimpleMath::Vector3();
}

DirectX::SimpleMath::Vector3 Camera::getForward() const
{
	return DirectX::SimpleMath::Vector3();
}

DirectX::SimpleMath::Vector3 Camera::getUp() const
{
	return DirectX::SimpleMath::Vector3();
}

DirectX::SimpleMath::Vector3 Camera::getRight() const
{
	return DirectX::SimpleMath::Vector3();
}
