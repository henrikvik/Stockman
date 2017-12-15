#include "Player/Weapon/Weapon.h"
#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

Weapon::Weapon()
{
    m_projectileData = ProjectileData();
}

Weapon::Weapon(ProjectileManager* projectileManager, ProjectileData& projectileData, WeaponInfo wInfo)
{
    m_wInfo = wInfo;
    m_projectileData = projectileData;

    setSpawnFunctions(*projectileManager);
}

Weapon::~Weapon()
{
}

void Weapon::setSpawnFunctions(ProjectileManager &projManager)
{
    spawnProjectile = [&](ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter, btVector3 modelOffset) -> Projectile* {
        return projManager.addProjectile(pData, position, forward, shooter, modelOffset);
    };
}

void Weapon::use(btVector3 position, float yaw, float pitch, Entity& shooter)
{
    std::vector<Projectile*> firedProjectiles;
    firedProjectiles.reserve(m_wInfo.projectileCount);

	// Use weapon
	if (m_wInfo.spreadH != 0 || m_wInfo.spreadV != 0)	// Spread
	{
		for (int i = m_wInfo.projectileCount; i--; )
		{
			btVector3 projectileDir = calcSpread(yaw, pitch);
			Projectile* p = spawnProjectile(m_projectileData, position, projectileDir, shooter, m_wInfo.projectileOffset);
            if (p)
            {
                firedProjectiles.push_back(p);

                auto speed = DirectX::SimpleMath::Vector3(m_projectileData.speed);
                auto pos = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3{}, p->getModelTransformMatrix());

                if (m_projectileData.hasEffect && !m_projectileData.effectActivated)
                    Graphics::FXSystem->processEffect(&m_projectileData.effect, pos, speed * DirectX::SimpleMath::Vector3{ projectileDir.x(), projectileDir.y(), projectileDir.z() }, 0.16f);
            }
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
			Projectile* p = spawnProjectile(m_projectileData, position, projectileDir, shooter, m_wInfo.projectileOffset);
            if (p)
            {
                firedProjectiles.push_back(p);

                auto speed = DirectX::SimpleMath::Vector3(m_projectileData.speed);
                auto pos = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3{}, p->getModelTransformMatrix());

                if (m_projectileData.hasEffect && !m_projectileData.effectActivated)
                    Graphics::FXSystem->processEffect(&m_projectileData.effect, pos, speed * DirectX::SimpleMath::Vector3{ projectileDir.x(), projectileDir.y(), projectileDir.z() }, 0.16f);
            } 
		}
	}

    onUse(firedProjectiles, shooter);
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

SpawnProjectile Weapon::getSpawnProjectileFunc()
{
    return spawnProjectile;
}

ProjectileData& Weapon::getProjectileData()
{
	return m_projectileData;
}

float Weapon::getAttackTimer(float modifier)
{
    if (m_wInfo.attackRate != 0)
       return (60.f / m_wInfo.attackRate) * 1000 * modifier;
    else
        return 0.f;
}

int Weapon::getDelayTime()
{
    return m_wInfo.delayTime;
}

float Weapon::getSwapTimer()
{
    return m_wInfo.swapTimer;
}