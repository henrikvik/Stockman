#pragma once

#include <d3d11.h>

#include <SimpleMath.h>
#include <Engine\Constants.h>

class DirectionalLight
{
public:
	DirectionalLight(ID3D11Device* device, int width, int height);
	~DirectionalLight();

	void update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context);

	ID3D11Buffer* getMatrixBuffer() { return matrixBuffer; };
	ID3D11Buffer* getShaderBuffer() { return shaderBuffer; };
	D3D11_VIEWPORT getViewPort() { return viewPort; };

private:
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;

	struct ShaderMatrix
	{
		DirectX::SimpleMath::Matrix vp;
	};

	struct ShaderValues
	{
		DirectX::SimpleMath::Vector4 pos;
	};

	ShaderMatrix matrixData;
	ShaderValues shaderData;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* shaderBuffer;

	D3D11_VIEWPORT viewPort;

};