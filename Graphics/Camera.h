#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "SimpleMath.h"

class Camera
{
public:
	Camera();
	~Camera();

	void setPos(DirectX::SimpleMath::Vector3 pos);
	void setForward(DirectX::SimpleMath::Vector3 forward);
	void setUp(DirectX::SimpleMath::Vector3 up);
	void setRight(DirectX::SimpleMath::Vector3 right);

	DirectX::SimpleMath::Vector3 getPos() const;
	DirectX::SimpleMath::Vector3 getForward() const;
	DirectX::SimpleMath::Vector3 getUp() const;
	DirectX::SimpleMath::Vector3 getRight() const;

private:
	DirectX::SimpleMath::Vector3 mPos;
	DirectX::SimpleMath::Vector3 mForward;
	DirectX::SimpleMath::Vector3 mUp;
	DirectX::SimpleMath::Vector3 mRight;	//Uncertain if needed

	float mDrawDistance;
	float mFieldOfView;
	float mFadeStart;



};