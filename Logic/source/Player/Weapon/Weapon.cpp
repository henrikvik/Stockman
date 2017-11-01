#include "Player/Weapon/Weapon.h"
#include <Physics\Physics.h>
#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>

using namespace Logic;

Weapon::Weapon(Graphics::ModelID modelID, ProjectileManager* projectileManager, ProjectileData &projectileData, int weaponID, int ammoCap, int ammo, int magSize, int magAmmo, int ammoConsumption, int projectileCount,
	int spreadH, int spreadV, float attackRate, float freeze, float reloadTime) : Object(modelID)
{
	m_weaponID			= weaponID;
	m_ammoCap			= ammoCap;
	m_ammo				= ammo;
	m_magSize			= magSize;
	m_magAmmo			= magAmmo;
	m_ammoConsumption	= ammoConsumption;
	m_projectileCount	= projectileCount;
	m_spreadH			= spreadH;
	m_spreadV			= spreadV;
	m_attackRate		= attackRate;
	m_freeze			= freeze;
	m_reloadTime		= reloadTime;
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

void Logic::Weapon::reset()
{
	m_ammo = m_ammoCap;
	m_magAmmo = m_magSize;
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
	if (m_spreadH != 0 || m_spreadV != 0)	// Spread
	{
		for (int i = m_projectileCount; i--; )
		{
			btVector3 projectileDir = calcSpread(yaw, pitch);
			SpawnProjectile(*m_projectileData, position, projectileDir, shooter);
		}
	}
	else									// No spread
	{
		for (int i = m_projectileCount; i--; )
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
	int rsh = (rand() % (2 * m_spreadH)) - m_spreadH;
	int rsv = (rand() % (2 * m_spreadV)) - m_spreadV;

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

int Weapon::getAmmoCap() const { return m_ammoCap; }

void Weapon::setAmmoCap(int ammoCap) { m_ammoCap = ammoCap; }

int Weapon::getAmmo() const { return m_ammo; }

void Weapon::setAmmo(int ammo) { m_ammo = ammo; }

int Weapon::getMagSize() const { return m_magSize; }

void Weapon::setMagSize(int magSize) { m_magSize = magSize; }

int Weapon::getMagAmmo() const { return m_magAmmo; }

void Weapon::removeMagAmmo() { m_magAmmo--; }

void Weapon::removeMagAmmo(int ammo) 
{ 
	if (ammo > m_magAmmo)
		m_magAmmo = 0;
	else
		m_magAmmo -= ammo; 
}

int Weapon::getAmmoConsumption() const { return m_ammoConsumption; }

float Weapon::getAttackTimer() const
{
	return (60.f / m_attackRate) * 1000;
}

float Weapon::getRealoadTime() const
{
	return m_reloadTime;
}

void Logic::Weapon::fillMag()
{
	int toAdd = m_magSize - m_magAmmo;

	if (m_ammo >= toAdd)
	{
		m_ammo -= toAdd;		// Remove ammo from total
		m_magAmmo = m_magSize;	// Add ammo to mag
	}
	else
	{
		m_magAmmo += m_ammo;	// Add rest of ammo to mag
		m_ammo = 0;				// Remove rest of ammo from total
	}
}