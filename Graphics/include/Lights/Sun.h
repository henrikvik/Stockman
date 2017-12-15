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

        struct DirectionalLight
        {
            DirectX::SimpleMath::Vector4 position;
            DirectX::SimpleMath::Vector3 color;
            private:float pad1;public:
            DirectX::SimpleMath::Vector3 ambient;
            private:float pad2;public:
        };

		Sun(ID3D11ShaderResourceView *shadowMap);
		~Sun();

		void update();

		ConstantBuffer<ShaderMatrix>* getLightMatrixBuffer() { return &lightMatrixBuffer; };
		ConstantBuffer<DirectionalLight>* getGlobalLightBuffer() { return &globalLightBuffer; };

	//private:
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		DirectX::SimpleMath::Vector4 pos;

		//Clamp a value between min and max
		float snap(float value, float min, float max);

        DirectX::SimpleMath::Vector3 m_LightDir;
        ID3D11ShaderResourceView *m_ShadowMap;
		ShaderMatrix matrixData;
        DirectionalLight globalLight;
		ConstantBuffer<ShaderMatrix> lightMatrixBuffer;
		ConstantBuffer<DirectionalLight> globalLightBuffer;
	};
}