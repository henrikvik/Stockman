#include "SnowManager.h"
#include <Logic\include\Misc\RandomGenerator.h>
#include <math.h>

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
		FrustumPlanes planes = generatePlanes(camera);

		addSnowFlake(planes, camera);
		
	}

	void SnowManager::addSnowFlake(FrustumPlanes& planes, Camera * camera)
	{
		Logic::RandomGenerator & random = Logic::RandomGenerator::singleton();

		Vector3 randVec(random.getRandomFloat(0.1, 20), random.getRandomFloat(0.1, 20), random.getRandomFloat(0.1, 20));
		
		Vector3 pointInPlane = randVec - (planes.farPlane.normal * (planes.farPlane.normal.Dot(randVec) / planes.farPlane.normal.LengthSquared()));
		pointInPlane = pointInPlane + (planes.farPlane.distance * planes.farPlane.normal);


		Vector3 snowPos = (pointInPlane - camera->getPos()) * random.getRandomFloat(0.01f, 1.f);


		positions.push_back(snowPos);
		velocities.push_back({0, -1, 0});
	}

	//this function randomizes snow all over the frustum because otherwise all snow will start from the top
	void SnowManager::initializeSnowflakes(Camera * camera)
	{
		positions.clear();
		velocities.clear();

		FrustumPlanes plane = generatePlanes(camera);


		for (int i = 0; i < MAX_SNOW; i++)
		{

		}
	}

	SnowManager::FrustumPlanes SnowManager::generatePlanes(Camera * camera)
	{

		Matrix v = camera->getView() * camera->getProj();

		Plane planeArr[6] =
		{
		{ { -(v._14 + v._11), -(v._24 + v._21), -(v._34 + v._31) }, { -(v._44 + v._41) } },
		{ { -(v._14 - v._11), -(v._24 - v._21), -(v._34 - v._31) }, { -(v._44 - v._41) } },
		{ { -(v._14 - v._12), -(v._24 - v._22), -(v._34 - v._32) }, { -(v._44 - v._42) } },
		{ { -(v._14 + v._12), -(v._24 + v._22), -(v._34 + v._32) }, { -(v._44 + v._42) } },
		{ { -(v._14 + v._13), -(v._24 + v._23), -(v._34 + v._33) }, { -(v._44 + v._43) } },
		{ { -(v._14 - v._13), -(v._24 - v._23), -(v._34 - v._33) }, { -(v._44 - v._43) } }
		};

		for (int i = 0; i < 6; i++)
		{
			float denom = 1.f / planeArr[i].normal.Length();

			planeArr[i].normal *= denom;
			planeArr[i].distance *= denom;
		}


		FrustumPlanes planes;
		planes.leftPlane	=	planeArr[0];
		planes.rightPlane	=	planeArr[1];
		planes.topPlane		=	planeArr[2];
		planes.bottomPlane	=	planeArr[3];
		planes.nearPlane	=   planeArr[4];
		planes.farPlane		=	planeArr[5];

		return planes;
	}
	
	bool SnowManager::isPointInFrontOfPlane(Plane plane, DirectX::SimpleMath::Vector3 point)
	{
		bool ret = false;
		Vector3 compare = plane.normal;
		compare *= plane.distance;

		compare = point - compare;

		if ((plane.normal).Dot(compare) < 0)
			ret = true;

		return ret;
	}
	
	bool SnowManager::isInFrustum(FrustumPlanes& planes, DirectX::SimpleMath::Vector3 point)
	{
		if (isPointInFrontOfPlane(planes.leftPlane, point))
			return false;

		if (isPointInFrontOfPlane(planes.rightPlane, point))
			return false;

		if (isPointInFrontOfPlane(planes.topPlane, point))
			return false;

		if (isPointInFrontOfPlane(planes.bottomPlane, point))
			return false;

		if (isPointInFrontOfPlane(planes.nearPlane, point))
			return false;

		if (isPointInFrontOfPlane(planes.farPlane, point))
			return false;


		return true;
	}
}