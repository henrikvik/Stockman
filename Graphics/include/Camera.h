#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <SimpleMath.h>
#include "Utility\ConstantBuffer.h"

namespace Graphics {
	class Camera
	{
	public:
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
		ID3D11Buffer* getBuffer();
		ID3D11Buffer* getInverseBuffer();

		void update(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 forward, ID3D11DeviceContext* context);
        void updateLookAt(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, ID3D11DeviceContext* context);
	private:
		DirectX::SimpleMath::Vector3 mPos;
		DirectX::SimpleMath::Vector3 mForward;
		DirectX::SimpleMath::Vector3 mUp;
		DirectX::SimpleMath::Vector3 mRight;	//Uncertain if needed

		float mDrawDistance;
		float mFieldOfView;
		float mFadeStart;

		DirectX::SimpleMath::Matrix mView;
		DirectX::SimpleMath::Matrix mProjection;

		struct ShaderValues {
			DirectX::SimpleMath::Matrix mVP;
			DirectX::SimpleMath::Matrix mInvP;
			DirectX::SimpleMath::Matrix mV;
			DirectX::SimpleMath::Vector4 camPos;
		} values;

		struct InverseMatrixes
		{
			DirectX::SimpleMath::Matrix mInvView;
			DirectX::SimpleMath::Matrix mInvP;

		} inverseMatrixes;

		ID3D11Buffer* mVPBuffer;
		ConstantBuffer<InverseMatrixes> inverseBuffer;
	};
}