#pragma once

#include <d3d11.h>

#include <SimpleMath.h>
#include <Engine\Constants.h>
class DirectionalLight
{
public:
	DirectionalLight(ID3D11Device* device, int width, int height);
	~DirectionalLight();

	void update(ID3D11DeviceContext* context, DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3(0, 0, 0));

	ID3D11Buffer* getMatrixBuffer() { return matrixBuffer; };
	ID3D11Buffer* getShaderBuffer() { return shaderBuffer; };
	D3D11_VIEWPORT getViewPort() { return viewPort; };

private:
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;
	DirectX::SimpleMath::Vector4 pos;

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