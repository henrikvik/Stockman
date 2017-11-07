#include "Sun.h"
#include <algorithm>
#define PI 3.14159265f
#define ONE_DEG_IN_RAD 0.01745f
#define SUNSET_TIME 0.5f
#define DAY_NIGHT_ON false

namespace Graphics
{
    Sun::Sun(int width, int height):
		lightMatrixBuffer(Global::device),
		lightDataBuffer(Global::device)
	{
		pos = DirectX::SimpleMath::Vector4(0, 50, 0.5, 1);

		projection = DirectX::XMMatrixOrthographicRH(100.f, 100.f, 1, 300);
		view = DirectX::XMMatrixLookAtRH(pos, DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 1, 0));

		matrixData.vp = view * projection;

		viewPort = { 0 };
		viewPort.Height = (float)height;
		viewPort.Width = (float)width;
		viewPort.MaxDepth = 1.f;
	}

	Sun::~Sun()
	{
	}

	void Sun::update()
	{
		//a little bit temp, might be final totally final now
		static float rotationDeg = rotationDeg = PI * 0.25f;

        DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationZ(rotationDeg);

		this->shaderData.pos = DirectX::SimpleMath::Vector4::Transform(pos, rotation);

		//If its nighttime the shadows fade out
        DirectX::SimpleMath::Vector3 lightDir = -shaderData.pos;
		lightDir.Normalize();

        DirectX::SimpleMath::Vector3 groundDir(1, 0, 0);

		float fade = snap(1.0f - abs(lightDir.Dot(groundDir)), 0, SUNSET_TIME);
		shaderData.fade = fade / SUNSET_TIME;

		this->shaderData.pos = shaderData.pos + campos;
		view = DirectX::XMMatrixLookAtRH(shaderData.pos, campos, DirectX::SimpleMath::Vector3(0, 1, 0));
		matrixData.vp = view * projection;

		lightDataBuffer.write(Global::context, &shaderData, sizeof(shaderData));
		lightMatrixBuffer.write(Global::context, &matrixData, sizeof(matrixData));
	}

	float Sun::snap(float value, float minVal, float maxVal)
	{
		value = max(min(value, maxVal), minVal);
		return value;
	}
}