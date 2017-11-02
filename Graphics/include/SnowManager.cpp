#include "SnowManager.h"
#include <Logic\include\Misc\RandomGenerator.h>
#include <math.h>
#include <CommonStates.h>
#include <Engine\Constants.h>
#define SNOW_RADIUS 50.f
#define MAX_SNOW 512
#define PI 3.14159265f
#define ONE_DEG_IN_RAD 0.01745f

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
        Logic::RandomGenerator & generator = Logic::RandomGenerator::singleton();

        static float windTimer = 5000;
        static float windCounter = 0;
        static Vector3 randWindPrev(0, -1, 0);
        static Vector3 randWind(0, -1, 0);
        static float friction = 0.6f;

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

        windCounter += deltaTime;
        if (windTimer <= windCounter)
        {
            randWindPrev = randWind;
            windCounter = 0;
            randWind.x = generator.getRandomFloat(-1, 1);
            randWind.z = generator.getRandomFloat(-1, 1);
            randWind.y = -1;
        }

		for (int i = 0; i < MAX_SNOW; i++)
		{
			if ((snowFlakes[i].position - camera->getPos()).Length() > SNOW_RADIUS)
				moveSnowFlake(camera, i);

			snowFlakes[i].distance = (snowFlakes[i].position - camera->getPos()).Length();
            snowFlakes[i].randomRot += generator.getRandomFloat(0, ONE_DEG_IN_RAD * 5);

            

			snowFlakes[i].position += Vector3::Lerp(randWindPrev, randWind, windCounter / windTimer) * deltaTime * 0.01f;



		}
		
		snowBuffer.write(context, &snowFlakes[0], snowFlakeCount * sizeof(SnowFlake));

	}

	void SnowManager::addRandomSnowFlake(Camera * camera)
	{
	    Logic::RandomGenerator & generator = Logic::RandomGenerator::singleton();

		Vector3 randVec(generator.getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), generator.getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS), generator.getRandomFloat(-SNOW_RADIUS, SNOW_RADIUS));

		Vector3 finalPos = camera->getPos() + randVec;

		SnowFlake flake;
		flake.position = finalPos;
		flake.randomRot = generator.getRandomFloat(0, PI * 2.f);
		flake.distance = randVec.Length();

		snowFlakes.push_back(flake);

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

	void SnowManager::drawSnowflakes(ID3D11DeviceContext * context, Camera * camera, ID3D11RenderTargetView * target, DepthStencil * depthMap, SkyRenderer& sky)
	{
		context->GSSetConstantBuffers(0, 1, *camera->getBuffer());
		context->VSSetShaderResources(4, 1, snowBuffer);

        context->PSSetConstantBuffers(1, 1, *sky.getShaderBuffer());
        context->GSSetConstantBuffers(3, 1, *sky.getLightMatrixBuffer());
        context->PSSetShaderResources(3, 1, *sky.getDepthStencil());
        context->OMSetRenderTargets(1, &target, *depthMap);

        context->PSSetSamplers(1, 1, sky.getSampler());

        context->IASetInputLayout(nullptr);
		context->VSSetShader(snowShader, nullptr, 0);
		context->PSSetShader(snowShader, nullptr, 0);
		context->GSSetShader(snowShader, nullptr, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		
        context->Draw(snowFlakeCount, 0);

        ID3D11ShaderResourceView * nullSRV = nullptr;
        ID3D11RenderTargetView * nullRTV = nullptr;


        context->OMSetRenderTargets(1, &nullRTV, nullptr);
        context->PSSetShaderResources(3, 1, &nullSRV);
	}

	void SnowManager::recompile(ID3D11Device * device)
	{
		snowShader.recompile(device, SHADER_PATH("SnowShaders/SnowShader.hlsl"), {}, ShaderType::GS | ShaderType::PS | ShaderType::VS);
	}

	void SnowManager::clearSnow()
	{
		snowFlakes.clear();
		snowFlakeCount = 0;
	}
}