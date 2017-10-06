#include <Graphics\include\Camera.h>
using namespace DirectX::SimpleMath;
using namespace Graphics;

Camera::Camera(ID3D11Device* device, int width, int height, float drawDistance, float fieldOfView):
	inverseBuffer(device)
{
	this->mFieldOfView = fieldOfView;
	this->mDrawDistance = drawDistance;

	this->mProjection = DirectX::XMMatrixPerspectiveFovRH(fieldOfView, float(width) / height, 0.1f, drawDistance);

	values.mVP = this->mProjection * this->mView;
	values.mV = this->mView;
	values.mInvP = this->mProjection.Invert();

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(ShaderValues);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &values;

	device->CreateBuffer(&desc, &data, &this->mVPBuffer);
}

Camera::~Camera()
{
	this->mVPBuffer->Release();
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

DirectX::SimpleMath::Matrix Graphics::Camera::getView() const
{
	return mView;
}

DirectX::SimpleMath::Matrix Graphics::Camera::getProj() const
{
	return mProjection;
}

ID3D11Buffer* Graphics::Camera::getBuffer()
{
	return this->mVPBuffer;
}

ID3D11Buffer * Graphics::Camera::getInverseBuffer()
{
	return this->inverseBuffer;
}

#include <Mouse.h>

void Graphics::Camera::update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context)
{
	forward.Normalize();

	this->mView = DirectX::XMMatrixLookToRH(pos, forward, Vector3(0, 1, 0));
	
	
	this->mPos = pos;

	values.mV = this->mView;
	values.mVP = this->mView * this->mProjection;
	values.mInvP = this->mProjection.Invert();
	values.camPos = Vector4(pos.x, pos.y, pos.z, 1);

	inverseMatrixes.mInvP = values.mInvP;
	inverseMatrixes.mInvView = mView.Invert();

	inverseBuffer.write(context, &inverseMatrixes, sizeof(InverseMatrixes));

	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(data));

	context->Map(this->mVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	memcpy(data.pData, &values, sizeof(ShaderValues));

	context->Unmap(this->mVPBuffer, 0);
	
}

void Graphics::Camera::updateLookAt(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, ID3D11DeviceContext * context)
{
    Matrix newView = DirectX::XMMatrixLookAtRH(pos, target, Vector3(0, 1, 0));

  
    this->mView = newView;
    this->mPos = pos;

    values.mVP = this->mView * this->mProjection;
    values.mInvP = this->mProjection.Invert();
    values.mV = this->mView;
    values.camPos = Vector4(pos.x, pos.y, pos.z, 1);

    D3D11_MAPPED_SUBRESOURCE data;
    ZeroMemory(&data, sizeof(data));

    context->Map(this->mVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

    memcpy(data.pData, &values, sizeof(ShaderValues));

    context->Unmap(this->mVPBuffer, 0);
    
}
