#pragma once
#include "Resources\Shader.h"
#include "Utility\DepthStencil.h"
#include <Engine\Constants.h>


class Fog
{
public:
	Fog(ID3D11Device* device);
	~Fog();

	void createShaders(ID3D11Device* device);
	void setFog(ID3D11DeviceContext * context);
private:
	ID3D11VertexShader* fog_VS_Shader;
	ID3D11PixelShader* fog_PS_Shader;
	ID3DBlob* vsTest;

};