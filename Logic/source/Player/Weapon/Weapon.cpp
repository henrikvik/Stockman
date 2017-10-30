#include "Player/Weapon/Weapon.h"
#include <Physics\Physics.h>
#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>

using namespace Logic;

Weapon::Weapon()
{
    m_projectileData = new ProjectileData();
}

Weapon::Weapon(Graphics::ModelID modelID, ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo) : Object(modelID)
{
    m_wInfo = wInfo;
    m_projectileData    = new ProjectileData(projectileData);

    setSpawnFunctions(*projectileManager);

	/////////////////////////////////////////////////////////////
	// Weapon model - These are the constant matrices that moves the 
	//					model a bit to the right and down & rotates a bit
	// Pointing the gun upwards
	//rotX = DirectX::SimpleMath::Matrix::CreateRotationX(10.0f * (3.14 / 180));

	// Tilting the gun to the middle
	//rotY = DirectX::SimpleMath::Matrix::CreateRotationY(15.f * (3.14 / 180));

	// Moving the model down to the right
	// trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.f, -2.25f, 0.f));
    trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.107f, -1.592f, -9.159f));

	// Scaling the model by making it thinner and longer
	scale = DirectX::SimpleMath::Matrix::CreateScale(0.197f, 0.199f, 0.097f);

	rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.15f, 0.15f, 0.05f);

}

Weapon::~Weapon()
{
    delete m_projectileData;
}

void Weapon::setSpawnFunctions(ProjectileManager &projManager)
{
    SpawnProjectile = [&](ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter) -> Projectile* {
        return projManager.addProjectile(pData, position, forward, shooter);
    };
}

void Weapon::use(btVector3 position, float yaw, float pitch, Entity& shooter)
{
	// Use weapon
	if (m_wInfo.spreadH != 0 || m_wInfo.spreadV != 0)	// Spread
	{
		for (int i = m_wInfo.projectileCount; i--; )
		{
			btVector3 projectileDir = calcSpread(yaw, pitch);
			SpawnProjectile(*m_projectileData, position, projectileDir, shooter);
		}
	}
	else									// No spread
	{
		for (int i = m_wInfo.projectileCount; i--; )
		{
			btVector3 projectileDir;
			projectileDir.setX(cos(DirectX::XMConvertToRadians(pitch)) * cos(DirectX::XMConvertToRadians(yaw)));
			projectileDir.setY(sin(DirectX::XMConvertToRadians(pitch)));
			projectileDir.setZ(cos(DirectX::XMConvertToRadians(pitch)) * sin(DirectX::XMConvertToRadians(yaw)));
			SpawnProjectile(*m_projectileData, position, projectileDir, shooter);
		}
	}
}

btVector3 Logic::Weapon::calcSpread(float yaw, float pitch)
{
	int rsh = (rand() % (2 * m_wInfo.spreadH)) - m_wInfo.spreadH;
	int rsv = (rand() % (2 * m_wInfo.spreadV)) - m_wInfo.spreadV;

	yaw += rsh;
	pitch += rsv;

	btVector3 projectileDir;
	projectileDir.setX(cos(DirectX::XMConvertToRadians(pitch)) * cos(DirectX::XMConvertToRadians(yaw)));
	projectileDir.setY(sin(DirectX::XMConvertToRadians(pitch)));
	projectileDir.setZ(cos(DirectX::XMConvertToRadians(pitch)) * sin(DirectX::XMConvertToRadians(yaw)));

	return projectileDir;
}

void Weapon::setWeaponModelFrontOfPlayer(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
	static DirectX::SimpleMath::Matrix camera, result, offset;

	//static float ltrans[3];
	//static float lscale[3] = {1, 1, 1};
	//static float lrot[3];

	//ImGui::Begin("asdoasdoasod");

	//ImGui::DragFloat3("translate", ltrans, 0.1f);
	//ImGui::DragFloat3("scale", lscale, 0.1f);
	//ImGui::DragFloat3("rotation", lrot, 0.1f);

	//ImGui::End();

	//trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(ltrans));

	//// Scaling the model by making it thinner and longer
	//scale = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(lscale));
	//auto rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(lrot[0], lrot[1], lrot[2]);

	// Making a camera matrix and then inverting it 
	camera = DirectX::XMMatrixLookToRH({0, 0, 0}, playerForward, { 0, 1, 0 });

	// Pushing the model forward in the current view direction
	offset = (DirectX::SimpleMath::Matrix::CreateTranslation(playerTranslation.Translation() + playerForward * -0.4f));
	
	// Multiplying all the matrices into one
	result = rot *trans * scale * camera.Invert() * offset;

	this->setWorldTranslation(result);
}

ProjectileData* Weapon::getProjectileData()
{
	return m_projectileData;
}

float Weapon::getAttackTimer()
{
	return (60.f / m_wInfo.attackRate) * 1000;
}