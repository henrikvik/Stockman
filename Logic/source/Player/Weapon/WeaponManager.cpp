#include "Player/Weapon/WeaponManager.h"
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Player\Weapon\WeaponMeleeParry.h>

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
	m_attackRateTimer = -1.f;
    m_toUse = USE_NOTHING;
    m_toUseShooter = nullptr;

	m_reloadTimer = 0.f;
	m_reloadState = ReloadingWeapon::IDLE;

    m_Upgrades.magSizeModifier = 0;
    m_Upgrades.ammoCapModifier = 0;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0.0f;
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
 //   // Reset ammo
	//for (int i = 0; i < m_weaponLoadouts.size(); i++)
 //       m_weaponLoadouts[i]->ammoContainer->reset();
    clear();

    // Reset current weapon
    m_currentWeapon = m_weaponLoadouts[0];
}

void WeaponManager::setWeaponModel(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    // Updating weapon model
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

void Logic::WeaponManager::affect(Effect const & effect)
{
    long long flags = effect.getStandards()->flags;

    if(flags & Effect::EFFECT_INCREASE_AMMOCAP)
    {
        m_Upgrades.ammoCapModifier = effect.getModifiers()->modifyAmmoCap;
    }
    if (flags & Effect::EFFECT_INCREASE_MAGSIZE)
    {
        m_Upgrades.magSizeModifier = effect.getModifiers()->modifyMagCap;
    }
    if (flags & Effect::EFFECT_INCREASE_FIRERATE)
    {
        m_Upgrades.fireRateModifier = effect.getModifiers()->modifyFirerate;
    }

}

void WeaponManager::render() const
{
    m_currentWeapon->weaponModel->render();
}

void WeaponManager::initializeWeapons(ProjectileManager* projectileManager)
{
    // Adding current weapon pairs
    m_weaponLoadouts =
    {
        // Gattling
        newd WeaponLoadout{
        /* Primary */       newd Weapon(projectileManager, ProjectileData(25, 0.2f, 1, 100, 0.f, 3000, Resources::Models::CrossBowProjectile, 1), Weapon::WeaponInfo{ 0, 1, 0, 0, 450, 0, 0 }),
        /* Secondary*/      newd Weapon(projectileManager, ProjectileData(10, 0.1f, 1, 100, 0.f, 500, Resources::Models::CrossBowProjectile, 1), Weapon::WeaponInfo{ 1, 36, 15, 10, 100, 0, 0 }),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 60, 60, 20, 20, 1, 5, 1000 }),
        /* WeaponModel */   newd WeaponModel(Resources::Models::Crossbow, WeaponModel::WeaponModelAnimationInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.05f, 0.12f, 0.05f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.5f, -1.8f, 4.6f)), DirectX::SimpleMath::Matrix::CreateScale(0.2f, 0.2f, 0.1f), 800.f }) },

        // Ice
        newd WeaponLoadout{
        /* Primary */       newd Weapon(projectileManager, ProjectileData(0, 1, 1, 20, 0, 675, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeIce, true), Weapon::WeaponInfo{ 2, 1, 17, 5, 750, 0, 1 }),
        /* Secondary*/      newd WeaponFreezeGrenade(projectileManager, ProjectileData(40, 0.6f, 1, 100, 0, 5000, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeFreezeGrenade), Weapon::WeaponInfo{ 3, 1, 0, 0, 50, 0, 0 }, ProjectileData(10, 0.2f, 1, 10, 5, 5000, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeNormal), 8),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 300, 300, 100, 100, 1, 25, 1500 }),
        /* WeaponModel */   newd WeaponModel(Resources::Models::Staff, WeaponModel::WeaponModelAnimationInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(-0.6f, -6.6f, 0.3f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(2.5f, -3.6f, 5.2f)), DirectX::SimpleMath::Matrix::CreateScale(0.2f, 0.2f, 0.2f), 800.f }) },

        // Sledge/Melee
        newd WeaponLoadout{ 
        /* Primary */       newd Weapon(projectileManager, ProjectileData(35, 2.f, 1, 0, 0, 0, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMelee, true, false, false), Weapon::WeaponInfo{ 4, 1, 0, 0, 400, 200, 0 }),
        /* Secondary*/      newd WeaponMeleeParry(projectileManager, ProjectileData(0, 2.f, 1, 0, 0, 200, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMeleeParry, true, false, true), Weapon::WeaponInfo{ 5, 1, 0, 0, 50, 0, 0 }, 8.f),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 0, 0, 0, 0, 0, 0, 0 }),
        /* WeaponModel */   newd WeaponModel(Resources::Models::Hammer, WeaponModel::WeaponModelAnimationInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(-1.7f, 0.2f, 3.2f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(1.3f, -0.2f, 2.f)), DirectX::SimpleMath::Matrix::CreateScale(0.2f, 0.2f, 0.2f), 200.f }) }
    };
}

void WeaponManager::switchWeapon(int index)
{
	// Check if swapTimer is active or swapping to same weapon 
	if (!(m_currentWeapon == m_weaponLoadouts[index]))
	{
		m_currentWeapon = m_weaponLoadouts[index];
		m_attackRateTimer = m_currentWeapon->weaponModel->getModelInfo().swapTimer;
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
        if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().primAmmoConsumption == 0)
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
        else
        {
            reloadWeapon();
        }
    }
}

void WeaponManager::usePrimary(btVector3 position, float yaw, float pitch, Entity& shooter)
{
    m_currentWeapon->primary->use(position, yaw, pitch, shooter);
    m_currentWeapon->ammoContainer->removePrimaryAmmo();

    m_attackRateTimer = m_currentWeapon->primary->getAttackTimer();
}

void WeaponManager::tryUseSecondary(btVector3 position, float yaw, float pitch, Player& shooter)
{
    if (m_attackRateTimer <= 0.f)
    {
        if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().secAmmoConsumption == 0)
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
        else
            reloadWeapon();
    }
}

void WeaponManager::useSecondary(btVector3 position, float yaw, float pitch, Entity& shooter)
{
    m_currentWeapon->secondary->use(position, yaw, pitch, shooter);
    m_currentWeapon->ammoContainer->removeSecondaryAmmo();

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
    return m_attackRateTimer > 0.f;
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
    if (m_weaponLoadouts[1] == m_currentWeapon || m_weaponLoadouts[2] == m_currentWeapon)
    {
        return m_weaponLoadouts[0];
    }
    else
    {
        return m_weaponLoadouts[1];
    };
}
