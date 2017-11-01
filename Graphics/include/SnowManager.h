#pragma once
#include "Camera.h"
#include "Resources\Shader.h"
#include "Utility\StructuredBuffer.h"
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
		void drawSnowflakes(ID3D11DeviceContext * context, Camera * camera);
		void recompile(ID3D11Device * device);

		//temp
		std::vector<DirectX::SimpleMath::Vector3> test;
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

		StructuredBuffer<DirectX::SimpleMath::Vector4> snowBuffer;
		Shader snowShader;

		int snowFlakeCount;


		std::vector<DirectX::SimpleMath::Vector4> positions;
		std::vector<DirectX::SimpleMath::Vector3> velocities;

		void addSnowFlake(FrustumPlanes& planes, Camera * camera);
		void addRandomSnowFlake(FrustumPlanes& planes, Camera * camera);
		void moveSnowFlake(FrustumPlanes& planes, Camera * camera, int snowFlake);
		FrustumPlanes generatePlanes(Camera * camera);
		bool isPointInFrontOfPlane(Plane plane, DirectX::SimpleMath::Vector3 point);
		bool isInFrustum(FrustumPlanes& planes, DirectX::SimpleMath::Vector3 point);
		void clearSnow();
	};
}