#include "Sun.h"
#include <algorithm>
#define PI 3.14159265
#define SUNSET_TIME 0.5f
#define DAY_NIGHT_ON false

namespace Graphics
{
    Sun::Sun(ID3D11Device * device, int width, int height):
		matrixBuffer(device),
		shaderBuffer(device)
	{
		pos = DirectX::SimpleMath::Vector4(0, 50, 0.5, 1);

		projection = DirectX::XMMatrixOrthographicRH(100.f, 100.f, 1, 300);
		view = DirectX::XMMatrixLookAtRH(pos, DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 1, 0));

		matrixData.vp = view * projection;

		viewPort = { 0 };
		viewPort.Height = height;
		viewPort.Width = width;
		viewPort.MaxDepth = 1.f;
	}

	Sun::~Sun()
	{
	}

	void Sun::update(ID3D11DeviceContext * context, float rotationAmount, DirectX::SimpleMath::Vector3 offset)
	{
		//a little bit temp, might be final
		static float rotationDeg = 0;

		//Enable to get the day night cycle
#if DAY_NIGHT_ON
		rotationDeg += rotationAmount;
#else
		rotationDeg = PI * 0.25;
#endif

		if (rotationDeg >= PI * 0.5)
			rotationDeg = -PI * 0.5;

        DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationZ(rotationDeg);

		this->shaderData.pos = DirectX::SimpleMath::Vector4::Transform(pos, rotation);

		//If its nighttime the shadows fade out
        DirectX::SimpleMath::Vector3 lightDir = -shaderData.pos;
		lightDir.Normalize();

        DirectX::SimpleMath::Vector3 groundDir(1, 0, 0);

		float fade = snap(1.0 - abs(lightDir.Dot(groundDir)), 0, SUNSET_TIME);
		shaderData.fade = fade / SUNSET_TIME;

		this->shaderData.pos = shaderData.pos + offset;
		view = DirectX::XMMatrixLookAtRH(shaderData.pos, offset, DirectX::SimpleMath::Vector3(0, 1, 0));
		matrixData.vp = view * projection;

		shaderBuffer.write(context, &shaderData, sizeof(shaderData));
		matrixBuffer.write(context, &matrixData, sizeof(matrixData));
	}

	float Sun::getShadowFade() const
	{
		//return shaderData.shadowFade;
		return 0;
	}

	float Sun::snap(float value, float minVal, float maxVal)
	{
		value = max(min(value, maxVal), minVal);
		return value;
	}
}