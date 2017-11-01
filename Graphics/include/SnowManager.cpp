#include "SnowManager.h"
#include <Logic\include\Misc\RandomGenerator.h>
#include <math.h>
#include <Engine\Constants.h>

//temp
#include <Keyboard.h>

using namespace DirectX::SimpleMath;

namespace Graphics
{

	SnowManager::SnowManager(ID3D11Device * device) :
		snowBuffer(device, CpuAccess::Write, MAX_SNOW),
		snowShader(device, SHADER_PATH("SnowShaders/SnowShader.hlsl"), {}, ShaderType::GS | ShaderType::PS | ShaderType::VS)
	{
		test = std::vector<DirectX::SimpleMath::Vector3>(20);
	}

	SnowManager::~SnowManager()
	{
		GÖR EN SPHERE I STÄLLET
	}

	void SnowManager::updateSnow(float deltaTime, Camera * camera, ID3D11DeviceContext * context)
	{
		FrustumPlanes planes = generatePlanes(camera);

		//temp
		auto ks = DirectX::Keyboard::Get().GetState();

		static bool isP = false;
		static bool wasP = false;
		wasP = isP;
		isP = ks.P;

		if (!wasP && isP)
		{
			clearSnow();
			for (int i = 0; i < 1; i++)
			{
				addSnowFlake(planes, camera);
			}
		}

		/*for (int i = 0; i < 1; i++)
		{
			if (!isInFrustum(planes, positions[i]))
			{
				moveSnowFlake(planes, camera, i);
			}
		}*/

		//if (isPointInFrontOfPlane(planes.topPlane, positions[0]))
			//printf("utanfur");
		
		snowBuffer.write(context, &positions[0], snowFlakeCount * sizeof(Vector4));

	}

	void SnowManager::addSnowFlake(FrustumPlanes& planes, Camera * camera)
	{
		Logic::RandomGenerator & random = Logic::RandomGenerator::singleton();

		Vector3 randVec(random.getRandomFloat(-400, 400), random.getRandomFloat(0, 300), random.getRandomFloat(-400, 400));

		Vector3 pointInPlane = randVec - (planes.farPlane.normal * (planes.farPlane.normal.Dot(randVec) / planes.farPlane.normal.LengthSquared()));
		pointInPlane = pointInPlane - (planes.farPlane.distance * planes.farPlane.normal);

		Vector3 dist = (pointInPlane - camera->getPos()) * random.getRandomFloat(0.01f, 0.7f);
		Vector4 snowPos = Vector4(camera->getPos() + dist);
		snowPos.w = 1;

		snowPos.y = camera->getPos().y + 20;

		positions.push_back(snowPos);
		velocities.push_back({ 0, -1, 0 });

		snowFlakeCount++;
	}

	void SnowManager::addRandomSnowFlake(FrustumPlanes & planes, Camera * camera)
	{
		Logic::RandomGenerator & random = Logic::RandomGenerator::singleton();

		Vector3 randVec(random.getRandomFloat(-400, 400), random.getRandomFloat(0, 300), random.getRandomFloat(-400, 400));

		Vector3 pointInPlane = randVec - (planes.farPlane.normal * (planes.farPlane.normal.Dot(randVec) / planes.farPlane.normal.LengthSquared()));
		pointInPlane = pointInPlane - (planes.farPlane.distance * planes.farPlane.normal);

		Vector3 dist = (pointInPlane - camera->getPos()) * random.getRandomFloat(0.1f, 0.7f);
		Vector4 snowPos = Vector4(camera->getPos() + dist);
		snowPos.w = 1;

		positions.push_back(snowPos);
		velocities.push_back({ 0, -1, 0 });

		snowFlakeCount++;
	}

	void SnowManager::moveSnowFlake(FrustumPlanes & planes, Camera * camera, int snowFlake)
	{
		//TESTA DETTA I STÄLLET
		Vector4 newPos = Vector4::Transform(Vector4(positions[snowFlake].x, positions[snowFlake].y, positions[snowFlake].z, 1), camera->getView() * camera->getProj());
		newPos /= newPos.w;

		if (newPos.x > 1)
		{
		}


		//if (isPointInFrontOfPlane(planes.leftPlane, positions[snowFlake]))
		//{
		//	Vector2 as(camera->getPos().x, camera->getPos().x);
		//	Vector2 ad(camera->getForward().x, camera->getForward().z);

		//	Vector2 bs(positions[snowFlake].x, positions[snowFlake].z);
		//	Vector2 bd(-planes.leftPlane.normal.x, -planes.leftPlane.normal.z);

		//	Vector2 delta = bs - as;
		//	
		//	float det = (bd.x * ad.y) - (bd.y * ad.x);
		//	float u = (delta.y * bd.x - delta.x * bd.y) / det;
		//	float v = (delta.y * ad.x - delta.x * ad.y) / det;

		//	if (u > 0 && v > 0)
		//	{
		//		Vector2 intersection = as + ad * v;
		//		
		//		Vector2 reflect = intersection - bs;
		//		Vector2 reflected = Vector2::Reflect(reflect, ad);

		//		Vector2 finalPos = reflect + reflected;

		//		positions[snowFlake].x = finalPos.x;
		//		positions[snowFlake].z = finalPos.y;
		//	}
		//}

	}

	//this function randomizes snow all over the frustum because otherwise all snow will start from the top
	void SnowManager::initializeSnowflakes(Camera * camera)
	{
		clearSnow();

		FrustumPlanes planes = generatePlanes(camera);


		for (int i = 0; i < MAX_SNOW; i++)
		{
			addRandomSnowFlake(planes, camera);
		}
	}

	void SnowManager::drawSnowflakes(ID3D11DeviceContext * context, Camera * camera)
	{
		context->GSSetConstantBuffers(0, 1, *camera->getBuffer());
		context->VSSetShaderResources(4, 1, snowBuffer);

		context->IASetInputLayout(nullptr);
		context->VSSetShader(snowShader, nullptr, 0);
		context->PSSetShader(snowShader, nullptr, 0);
		context->GSSetShader(snowShader, nullptr, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		context->Draw(snowFlakeCount, 0);
	}

	void SnowManager::recompile(ID3D11Device * device)
	{
		snowShader.recompile(device, SHADER_PATH("SnowShaders/SnowShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } }, ShaderType::GS | ShaderType::PS | ShaderType::VS);
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
		Vector3 compare = plane.normal * plane.distance;

		compare = point - compare;

		compare.Normalize();

		if ((plane.normal).Dot(compare) > 0)
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
	
	void SnowManager::clearSnow()
	{
		positions.clear();
		velocities.clear();
		snowFlakeCount = 0;
	}
}