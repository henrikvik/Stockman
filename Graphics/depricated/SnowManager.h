#pragma once
#include "Camera.h"
#include "Utility\Shader.h"
#include "Utility\StructuredBuffer.h"
#include "SkyRenderer.h"
#include <vector>

namespace Graphics 
{
	class SnowManager
	{
	public:
		SnowManager(ID3D11Device * device);
		~SnowManager();

		void updateSnow(float deltaTime, Camera * camera, ID3D11DeviceContext * context);
		void initializeSnowflakes(Camera * camera);
		void drawSnowflakes(ID3D11DeviceContext * context, Camera * camera, ID3D11RenderTargetView * target, DepthStencil * depthMap, SkyRenderer& sky);
		void recompile(ID3D11Device * device);


	private:
		struct SnowFlake
		{
			DirectX::SimpleMath::Vector3 position;
			float randomRot;
			float distance;
		};

		StructuredBuffer<SnowFlake> snowBuffer;
		Shader snowShader;
		int snowFlakeCount;


		std::vector<SnowFlake> snowFlakes;
		void addRandomSnowFlake(Camera * camera);
		void moveSnowFlake(Camera * camera, int snowFlake);
		void clearSnow();
	};
}