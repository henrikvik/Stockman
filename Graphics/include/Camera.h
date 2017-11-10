#pragma once

#include "Utility\ConstantBuffer.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <SimpleMath.h>

namespace Graphics {
	class Camera
	{
	public:
		struct ShaderValues {
			DirectX::SimpleMath::Matrix vP;
			DirectX::SimpleMath::Matrix invP;
			DirectX::SimpleMath::Matrix view;
            DirectX::SimpleMath::Vector4 camPos;
            DirectX::SimpleMath::Vector4 forward;
		} values;

		struct InverseMatrixes
		{
			DirectX::SimpleMath::Matrix invView;
			DirectX::SimpleMath::Matrix invP;

		};

		Camera(ID3D11Device* device, int width, int height, float drawDistance = 100, float fieldOfView = M_PI * 0.45);
		~Camera();

		void setPos(DirectX::SimpleMath::Vector3 pos);
		void setForward(DirectX::SimpleMath::Vector3 forward);
		void setUp(DirectX::SimpleMath::Vector3 up);
		void setRight(DirectX::SimpleMath::Vector3 right);

		DirectX::SimpleMath::Vector3 getPos() const;
		DirectX::SimpleMath::Vector3 getForward() const;
		DirectX::SimpleMath::Vector3 getUp() const;
		DirectX::SimpleMath::Vector3 getRight() const;
		DirectX::SimpleMath::Matrix getView() const;
		DirectX::SimpleMath::Matrix getProj() const;
		float getFarPlane() const { return drawDistance; };
		ConstantBuffer<ShaderValues>* getBuffer();
		ConstantBuffer<InverseMatrixes>* getInverseBuffer();

		void update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context);
        void updateLookAt(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, ID3D11DeviceContext* context);
	private:
		DirectX::SimpleMath::Vector3 pos;
		DirectX::SimpleMath::Vector3 forward;
		DirectX::SimpleMath::Vector3 up;
		DirectX::SimpleMath::Vector3 right;	//Uncertain if needed

		float drawDistance;
		float fieldOfView;
		float fadeStart;

		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;


		InverseMatrixes inverseMatrixes;

		ConstantBuffer<ShaderValues> cameraBuffer;
		ConstantBuffer<InverseMatrixes> inverseBuffer;
	};
}