#pragma once

#include "../Utility/ConstantBuffer.h"

#include <SimpleMath.h>
#include <Engine\Constants.h>
namespace Graphics
{
	class Sun
	{
	public:
		struct ShaderMatrix
		{
			DirectX::SimpleMath::Matrix vp;
			//"padding" TODO: Should probably make a new shader instead
			DirectX::SimpleMath::Matrix mInvP;
			//End padding
			DirectX::SimpleMath::Matrix mV;
			DirectX::SimpleMath::Vector4 camPos;
		};

		struct LightValues
		{
			DirectX::SimpleMath::Vector4 pos;
			//Value from 0 to 1, when it is 1 the shadows are on
			float fade;
			char pad[12];
		};

		Sun(int width, int height);
		~Sun();

		void update();

		ConstantBuffer<ShaderMatrix>* getLightMatrixBuffer() { return &lightMatrixBuffer; };
		ConstantBuffer<LightValues>* getLightDataBuffer() { return &lightDataBuffer; };
		D3D11_VIEWPORT getViewPort() { return viewPort; };

	private:
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		DirectX::SimpleMath::Vector4 pos;

		

		//Clamp a value between min and max
		float snap(float value, float min, float max);

		ShaderMatrix matrixData;
		LightValues shaderData;
		ConstantBuffer<ShaderMatrix> lightMatrixBuffer;
		ConstantBuffer<LightValues> lightDataBuffer;

		D3D11_VIEWPORT viewPort;

	};
}