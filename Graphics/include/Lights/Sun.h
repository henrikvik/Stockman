#pragma once

#include <d3d11.h>

#include <SimpleMath.h>
#include <Engine\Constants.h>
class Sun
{
public:
	struct ColorStruct
	{
		DirectX::SimpleMath::Vector3 dayColor;
		DirectX::SimpleMath::Vector3 sunDownColor;
		DirectX::SimpleMath::Vector3 nightColor;
	};


	Sun(ID3D11Device* device, int width, int height);
	~Sun();

	void update(ID3D11DeviceContext* context, float rotationAmount, DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3(0, 0, 0));

	ID3D11Buffer* getMatrixBuffer() { return matrixBuffer; };
	ID3D11Buffer* getShaderBuffer() { return shaderBuffer; };
	D3D11_VIEWPORT getViewPort() { return viewPort; };
	float getShadowFade() const;
	DirectX::SimpleMath::Vector3 getColor() const;

	//If this is never used, please remove it
	ColorStruct getColors() const;

private:
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;
	DirectX::SimpleMath::Vector4 pos;
	ColorStruct colors;
	bool isNight;

	struct ShaderMatrix
	{
		DirectX::SimpleMath::Matrix vp;
	};

	struct LightValues
	{
		DirectX::SimpleMath::Vector4 pos;
		DirectX::SimpleMath::Vector3 color;

		//Value from 0 to 1, when it is 1 the shadows are on
		float shadowFade;
	};

	//Clamp a value between min and max
	float snap(float value, float min, float max);

	ShaderMatrix matrixData;
	LightValues shaderData;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* shaderBuffer;

	D3D11_VIEWPORT viewPort;

};