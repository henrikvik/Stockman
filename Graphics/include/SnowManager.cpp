#include "SnowManager.h"
#define MAX_SNOW 256

using namespace DirectX::SimpleMath;

namespace Graphics
{

	SnowManager::SnowManager(ID3D11Device * device) :
		snowBuffer(device)
	{
	}

	SnowManager::~SnowManager()
	{
	}

	void SnowManager::updateSnow(float deltaTime, Camera * camera, ID3D11DeviceContext * context)
	{

	}

	void SnowManager::addSnowFlake(Camera * camera)
	{
	}

	//this function randomizes snow all over the frustum because otherwise all snow will start from the top
	void SnowManager::initializeSnowflakes(Camera * camera)
	{
		snowFlakes.clear();


		for (int i = 0; i < MAX_SNOW; i++)
		{

		}
	}

	SnowManager::FrustumPlane SnowManager::generatePlanes(Camera * camera)
	{
		FrustumPlane planes;

		Matrix * view = &camera->getView();

		planes.bottomPlane = Vector4(-view->_14 + view->_12, -view->_24 + view->_22, -view->_34 + view->_32, -view->_44 + view->_42);


		return planes;
	}
}