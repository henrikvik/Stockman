#pragma once
#include "Resources\Shader.h"
#include "Utility\DepthStencil.h"
#include <Engine\Constants.h>


class Fog
{
public:
	Fog();
	~Fog();

	void setFog(ID3D11DeviceContext * context);

};