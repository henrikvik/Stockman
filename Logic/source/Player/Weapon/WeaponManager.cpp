#include "Player/Weapon/WeaponManager.h"
#include <Player\Weapon\Weapon.h>
#include <Player\Player.h>
#include <Graphics\include\Structs.h>
#include <Projectile\ProjectileStruct.h>
#include <Player\Weapon\AmmoContainer.h>
#include <Player\Weapon\WeaponModel.h>

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { clear(); }

void WeaponManager::init(ProjectileManager* projectileManager)
{
	initializeWeapons(projectileManager);
	m_currentWeapon = m_weaponLoadouts[0];

	// Timers
	m_swapWeaponTimer = 1000.f;
	m_attackRateTimer = -1.f;
    m_toUse = USE_NOTHING;
    m_toUseShooter = nullptr;

	m_reloadTimer = 0.f;
	m_reloadState = ReloadingWeapon::IDLE;
}

void WeaponManager::clear()
{
    for (WeaponLoadout* weaponLoadout : m_weaponLoadouts)
    {
        delete weaponLoadout->primary;
        delete weaponLoadout->secondary;
        delete weaponLoadout->ammoContainer;
        delete weaponLoadout->weaponModel;
        delete weaponLoadout;
    }
        
	m_weaponLoadouts.clear();
}

void WeaponManager::reset()
{
	for (int i = 0; i < m_weaponLoadouts.size(); i++)
        m_weaponLoadouts[i]->ammoContainer->reset();
}

void WeaponManager::setWeaponModel(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
	// Updating weapon model
	//m_currentWeapon->primary->setWeaponModelFrontOfPlayer(playerTranslation, playerForward);
    m_currentWeapon->weaponModel->update(playerTranslation, playerForward);
}

void WeaponManager::update(float deltaTime)
{
	// Timers / cooldowns

	// Reload
	if (m_reloadTimer > 0.f)
	{
		m_reloadTimer -= deltaTime;
	}
	else if (m_reloadState == ReloadingWeapon::ACTIVE)
	{
		m_reloadState = ReloadingWeapon::DONE;
	}
	if (m_reloadState == ReloadingWeapon::DONE)
	{
		m_currentWeapon->ammoContainer->fillMag();
		m_reloadState = ReloadingWeapon::IDLE;
		printf("adding ammo\n");
		printf("ammo: %d\n", m_currentWeapon->ammoContainer->getAmmoInfo().ammo);
		printf("mag: %d\n", m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo);
	}

	// Attack
    if (m_attackRateTimer > 0.f)
    {
        m_attackRateTimer -= deltaTime;

        if (m_attackRateTimer < 0.f && m_toUse != USE_NOTHING)
        {
            if (m_toUse == USE_PRIMARY)
            {
                Entity* shooterEntity = m_toUseShooter;
                usePrimary(m_toUseShooter->getPositionBT() + m_toUseShooter->getForwardBT(), m_toUseShooter->getYaw(), m_toUseShooter->getPitch(), *shooterEntity);
                m_toUseShooter = nullptr;
            }
            else
            {
                Entity* shooterEntity = m_toUseShooter;
                useSecondary(m_toUseShooter->getPositionBT() + m_toUseShooter->getForwardBT(), m_toUseShooter->getYaw(), m_toUseShooter->getPitch(), *shooterEntity);
                m_toUseShooter = nullptr;
            }

            m_toUse = USE_NOTHING;
        }
    }

}

void WeaponManager::render(Graphics::Renderer& renderer)
{
    m_currentWeapon->weaponModel->render(renderer);
}

void WeaponManager::initializeWeapons(ProjectileManager* projectileManager)
{
    // Adding current weapon pairs
    m_weaponLoadouts =
    {
        // Gattling
        newd WeaponLoadout{ 
        /* Primary */       newd Weapon(projectileManager, ProjectileData(1, 0.2f, 1, 100, 0.f, 3000, Graphics::ModelID::CUTTLERY, 1), Weapon::WeaponInfo{ 0, 1, 0, 0, 450.f, 0.f, 0 }),
        /* Secondary*/      newd Weapon(projectileManager, ProjectileData(1, 0.1f, 1, 100, 0.f, 500, Graphics::ModelID::CUTTLERY, 1), Weapon::WeaponInfo{ 1, 18, 15, 10, 100.f, 0.f, 0 }),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 60, 60, 30, 30, 1, 6, 2000 }), 
        /* WeaponModel */   newd WeaponModel(Graphics::ModelID::CROSSBOW, WeaponModel::WeaponModelInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.15f, 0.15f, 0.05f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.107f, -1.592f, -9.159f)), DirectX::SimpleMath::Matrix::CreateScale(0.197f, 0.199f, 0.097f), -0.4f }) },

        // Ice
        newd WeaponLoadout{ 
        /* Primary */       newd Weapon(projectileManager, ProjectileData(0, 1, 1, 20, 0, 675, Graphics::ModelID::CUBE, 1, ProjectileType::ProjectileTypeIce, true), Weapon::WeaponInfo{ 2, 1, 17, 5, 750.f, 0.f, 1 }),
        /* Secondary*/      newd Weapon(projectileManager, ProjectileData(1, 1, 1, 50, 5, 5000, Graphics::ModelID::CUBE, 1), Weapon::WeaponInfo{ 3, 1, 0, 0, 100.f, 0.f, 1 }),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 300, 200, 100, 100, 1, 10, 3000 }),
        /* WeaponModel */   newd WeaponModel(Graphics::ModelID::STAFF, WeaponModel::WeaponModelInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.15f, 0.15f, 0.05f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.107f, -1.592f, -9.159f)), DirectX::SimpleMath::Matrix::CreateScale(0.197f, 0.199f, 0.097f), -0.4f }) },

        // Sledge/Melee
        newd WeaponLoadout{ 
        /* Primary */       newd Weapon(projectileManager, ProjectileData(1, 2, 1, 0, 0, 0, Graphics::ModelID::CUBE, 1, ProjectileType::ProjectileTypeMelee, true, false, false), Weapon::WeaponInfo{ 4, 1, 0, 0, 100.f, 200.f, 0 }),
        /* Secondary*/      newd Weapon(projectileManager, ProjectileData(1, 2, 1, 0, 0, 0, Graphics::ModelID::CUBE, 1, ProjectileType::ProjectileTypeMelee, true, false, false), Weapon::WeaponInfo{ 5, 1, 0, 0, 100.f, 200.f, 0 }),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 0, 0, 0, 0, 0, 0, 0 }),
        /* WeaponModel */   newd WeaponModel(Graphics::ModelID::STAFF, WeaponModel::WeaponModelInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.15f, 0.15f, 0.05f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.107f, -1.592f, -9.159f)), DirectX::SimpleMath::Matrix::CreateScale(0.197f, 0.199f, 0.097f), -0.4f }) }
    };
}

void WeaponManager::switchWeapon(int index)
{
	// Check if swapTimer is active or swapping to same weapon 
	if (!(m_currentWeapon == m_weaponLoadouts[index]))
	{
		m_currentWeapon = m_weaponLoadouts[index];
		m_attackRateTimer = m_swapWeaponTimer;
        m_toUse = USE_NOTHING;
        m_toUseShooter = nullptr;

		m_reloadTimer = 0.f;
		m_reloadState = ReloadingWeapon::IDLE;

		printf("switch weapon %d\n", index);
	}
}

void WeaponManager::tryUsePrimary(btVector3 position, float yaw, float pitch, Player& shooter)
{
    if (m_attackRateTimer <= 0.f)
    {
        if (float delayTime = m_currentWeapon->primary->getDelayTime())
        {
            m_toUse = USE_PRIMARY;
            m_toUseShooter = &shooter;
            m_attackRateTimer = delayTime;
        }
        else
        {
            Entity* shooterEntity = &shooter;
            usePrimary(position, yaw, pitch, *shooterEntity);
        }   
    }
}

void WeaponManager::usePrimary(btVector3 position, float yaw, float pitch, Entity& shooter)
{
    if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().primAmmoConsumption == 0)
    {
        m_currentWeapon->primary->use(position, yaw, pitch, shooter);
        m_currentWeapon->ammoContainer->removePrimaryAmmo();
    }
    else
        printf("out of ammo\n");

    m_attackRateTimer = m_currentWeapon->primary->getAttackTimer();
}

void WeaponManager::tryUseSecondary(btVector3 position, float yaw, float pitch, Player& shooter)
{
    if (m_attackRateTimer <= 0.f)
    {
        if (float delayTime = m_currentWeapon->secondary->getDelayTime())
        {
            m_toUse = USE_SECONDARY;
            m_toUseShooter = &shooter;
            m_attackRateTimer = delayTime;
        }
        else
        {
            Entity* shooterEntity = &shooter;
            useSecondary(position, yaw, pitch, *shooterEntity);
        }
    }
}

void WeaponManager::useSecondary(btVector3 position, float yaw, float pitch, Entity& shooter)
{
    if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().secAmmoConsumption == 0)
    {
        m_currentWeapon->secondary->use(position, yaw, pitch, shooter);
        m_currentWeapon->ammoContainer->removeSecondaryAmmo();
    }
    else
        printf("out of ammo\n");

    m_attackRateTimer = m_currentWeapon->secondary->getAttackTimer();
}

void WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f && m_currentWeapon->ammoContainer->getAmmoInfo().ammo > 0 && m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo < m_currentWeapon->ammoContainer->getAmmoInfo().magSize)
	{
        m_reloadTimer = m_currentWeapon->ammoContainer->getAmmoInfo().reloadTime;
		m_reloadState = ReloadingWeapon::ACTIVE;
		printf("reloading weapon\n");
	}
}

bool WeaponManager::isSwitching()
{
	return m_swapWeaponTimer > 0.f;
}

bool WeaponManager::isAttacking()
{
	return m_attackRateTimer > 0.f;
}

bool WeaponManager::isReloading()
{
	return m_reloadState != ReloadingWeapon::IDLE;
}

WeaponManager::WeaponLoadout* WeaponManager::getCurrentWeaponLoadout()
{
	return m_currentWeapon;
}

WeaponManager::WeaponLoadout* WeaponManager::getWeaponLoadout(int index)
{
    return m_weaponLoadouts[index];
}

WeaponManager::WeaponLoadout* WeaponManager::getActiveWeaponLoadout()
{
    if (m_weaponLoadouts[0] == m_currentWeapon)
    {
        return m_weaponLoadouts[0];
    }
    else
    {
        return m_weaponLoadouts[1];
    } 
}

WeaponManager::WeaponLoadout* WeaponManager::getInactiveWeaponLoadout()
{
    if (m_weaponLoadouts[1] == m_currentWeapon)
    {
        return m_weaponLoadouts[0];
    }
    else
    {
        return m_weaponLoadouts[1];
    };
}
