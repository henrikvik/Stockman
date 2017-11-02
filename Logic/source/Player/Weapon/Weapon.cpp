#include "Player/Weapon/Weapon.h"
#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>

using namespace Logic;

Weapon::Weapon()
{
    m_projectileData = new ProjectileData();
}

Weapon::Weapon(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo)
{
    m_wInfo = wInfo;
    m_projectileData = new ProjectileData(projectileData);

    setSpawnFunctions(*projectileManager);
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

ProjectileData* Weapon::getProjectileData()
{
	return m_projectileData;
}

float Weapon::getAttackTimer()
{
	return (60.f / m_wInfo.attackRate) * 1000;
}

float Weapon::getDelayTime()
{
    return m_wInfo.delayTime;
}
