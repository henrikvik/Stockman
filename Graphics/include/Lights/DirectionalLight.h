#pragma once

#include <d3d11.h>

#include <SimpleMath.h>
#include <Engine\Constants.h>

class DirectionalLight
{
public:
	DirectionalLight(ID3D11Device* device, int width, int height, float drawDistance = 100);
	~DirectionalLight();

	void update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context);

private:
	DirectX::SimpleMath::Matrix model;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;

	struct ShaderValues
	{
		DirectX::SimpleMath::Matrix mvp;
		DirectX::SimpleMath::Vector4 pos;
		DirectX::SimpleMath::Vector4 dir;
	};

	ShaderValues bufferStruct;
	ID3D11Buffer* buffer;

};