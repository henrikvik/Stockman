#pragma once
#include "Camera.h"
#include <vector>

namespace Graphics 
{
	class SnowManager
	{
	public:
		struct SnowFlake
		{
			DirectX::SimpleMath::Vector3 position;
		};

		DirectX::SimpleMath::Vector3 velocity;

		SnowManager(ID3D11Device * device);
		~SnowManager();

		void updateSnow(float deltaTime, Camera * camera, ID3D11DeviceContext * context);
		void initializeSnowflakes(Camera * camera);

	private:
		struct FrustumPlane
		{
			DirectX::SimpleMath::Vector4 leftPlane;
			DirectX::SimpleMath::Vector4 rightPlane;
			DirectX::SimpleMath::Vector4 upPlane;
			DirectX::SimpleMath::Vector4 bottomPlane;
			DirectX::SimpleMath::Vector4 farPlane;
		};

		ConstantBuffer<SnowFlake> snowBuffer;

		std::vector<SnowFlake> snowFlakes;

		void addSnowFlake(Camera * camera);
		FrustumPlane generatePlanes(Camera * camera);

	};
}