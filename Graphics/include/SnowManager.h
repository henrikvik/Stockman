#pragma once
#include "Camera.h"
#include <vector>
#define MAX_SNOW 256

namespace Graphics 
{
	class SnowManager
	{
	public:
		SnowManager(ID3D11Device * device);
		~SnowManager();

		void updateSnow(float deltaTime, Camera * camera, ID3D11DeviceContext * context);
		void initializeSnowflakes(Camera * camera);

	private:
		struct Plane
		{
			DirectX::SimpleMath::Vector3 normal;
			float distance;
		};
		struct FrustumPlanes
		{
			Plane leftPlane;
			Plane rightPlane;
			Plane topPlane;
			Plane bottomPlane;
			Plane nearPlane;
			Plane farPlane;
		};

		ConstantBuffer<DirectX::SimpleMath::Vector3, MAX_SNOW> snowBuffer;

		std::vector<DirectX::SimpleMath::Vector3> positions;
		std::vector<DirectX::SimpleMath::Vector3> velocities;

		void addSnowFlake(FrustumPlanes& planes, Camera * camera);
		FrustumPlanes generatePlanes(Camera * camera);
		bool isPointInFrontOfPlane(Plane plane, DirectX::SimpleMath::Vector3 point);
		bool isInFrustum(FrustumPlanes& planes, DirectX::SimpleMath::Vector3 point);
	};
}