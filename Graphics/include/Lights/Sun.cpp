#include "Sun.h"
#include <algorithm>
#include "../Device.h"
#include "../MainCamera.h"
#define PI 3.14159265f
#define ONE_DEG_IN_RAD 0.01745f
#define SUNSET_TIME 0.5f
#define DAY_NIGHT_ON false

namespace Graphics
{
    Sun::Sun():
		lightMatrixBuffer(Global::device),
		globalLightBuffer(Global::device)
	{
		pos = DirectX::SimpleMath::Vector4(0, 50, 0.5, 1);

		projection = DirectX::XMMatrixOrthographicRH(100.f, 100.f, 1, 300);
		view = DirectX::XMMatrixLookAtRH(pos, DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 1, 0));

		matrixData.vp = view * projection;

        globalLight.color = DirectX::SimpleMath::Vector3(0.1, 0.1, 0.3);
        globalLight.ambient = DirectX::SimpleMath::Vector3(0.2, 0.2, 0.2);
	}

	Sun::~Sun()
	{
	}

	void Sun::update()
	{
		//a little bit temp, might be final totally final now
		static float rotationDeg = rotationDeg = PI * 0.25f;

        DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationZ(rotationDeg);

		this->globalLight.position = DirectX::SimpleMath::Vector4::Transform(pos, rotation);

		//If its nighttime the shadows fade out
        DirectX::SimpleMath::Vector3 lightDir = -globalLight.position;
		lightDir.Normalize();

        DirectX::SimpleMath::Vector3 groundDir(1, 0, 0);


		this->globalLight.position = globalLight.position + Global::mainCamera->getPos();
		view = DirectX::XMMatrixLookAtRH(globalLight.position, Global::mainCamera->getPos(), DirectX::SimpleMath::Vector3(0, 1, 0));
		matrixData.vp = view * projection;

        globalLightBuffer.write(Global::context, &globalLight, sizeof(globalLight));
		lightMatrixBuffer.write(Global::context, &matrixData, sizeof(matrixData));
	}

	float Sun::snap(float value, float minVal, float maxVal)
	{
		value = max(min(value, maxVal), minVal);
		return value;
	}
}