#include "SnowManager.h"
#include <Logic\include\Misc\RandomGenerator.h>
#include <math.h>
#include <Engine\Constants.h>
#define SNOW_RADIUS 50.f
#define MAX_SNOW 256

//temp
#include <Keyboard.h>

using namespace DirectX::SimpleMath;

namespace Graphics
{

	SnowManager::SnowManager(ID3D11Device * device) :
		snowBuffer(device, CpuAccess::Write, MAX_SNOW),
		snowShader(device, SHADER_PATH("SnowShaders/SnowShader.hlsl"), {}, ShaderType::GS | ShaderType::PS | ShaderType::VS)
	{

	}

	SnowManager::~SnowManager()
	{
		
	}

	void SnowManager::updateSnow(float deltaTime, Camera * camera, ID3D11DeviceContext * context)
	{
		//temp
		auto ks = DirectX::Keyboard::Get().GetState();

		static bool isP = false;
		static bool wasP = false;
		wasP = isP;
		isP = ks.P;

		if (!wasP && isP)
		{
			initializeSnowflakes(camera);
		}

		for (int i = 0; i < MAX_SNOW; i++)
		{
			if ((snowFlakes[i].position - camera->getPos()).Length() > SNOW_RADIUS)
				moveSnowFlake(camera, i);

			snowFlakes[i].distance = (snowFlakes[i].position - camera->getPos()).Length();
		}
		
		snowBuffer.write(context, &snowFlakes[0], snowFlakeCount * sizeof(Vector4));

	}

	void SnowManager::addRandomSnowFlake(Camera * camera)
	{
	    Logic::RandomGenerator & generator = Logic::RandomGenerator::singleton();

		Vector3 randVec(generator.getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), generator.getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), generator.getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS));

		Vector3 finalPos = camera->getPos() + randVec;

		SnowFlake flake;
		flake.position = finalPos;
		flake.randomRot = generator.getRandomFloat(0, 360);
		flake.distance = randVec.Length();

		snowFlakes.push_back(flake);
		velocities.push_back(Vector3(0, -1, 0));

		snowFlakeCount++;
	}

	void SnowManager::moveSnowFlake(Camera * camera, int snowFlake)
	{
		Vector3 posToCam = camera->getPos() - snowFlakes[snowFlake].position;
		posToCam.Normalize();

		snowFlakes[snowFlake].position = camera->getPos() + (posToCam * SNOW_RADIUS);
	}

	//this function randomizes snow all over the frustum because otherwise all snow will start from the top
	void SnowManager::initializeSnowflakes(Camera * camera)
	{
		clearSnow();

		for (int i = 0; i < MAX_SNOW; i++)
		{
			addRandomSnowFlake(camera);
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

	void SnowManager::clearSnow()
	{
		snowFlakes.clear();
		velocities.clear();
		snowFlakeCount = 0;
	}
}