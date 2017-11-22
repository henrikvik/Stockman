#include "Player/Weapon/WeaponManager.h"
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\WeaponCrossbow.h>
#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Player\Weapon\WeaponMeleeParry.h>
#include <Misc\Sound\NoiseMachine.h>

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
    m_Upgrades.reloadTimeModifier = 1.0f;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0;
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
    // Reset ammo
	for (int i = 0; i < m_weaponLoadouts.size(); i++)
        m_weaponLoadouts[i]->ammoContainer->reset();
    

    // Reset current weapon
    m_currentWeapon = m_weaponLoadouts[0];

    m_attackRateTimer = 0;
    m_toUse = USE_NOTHING;
    m_toUseShooter = nullptr;

    m_reloadTimer = 0.f;
    m_reloadState = ReloadingWeapon::IDLE;

    //reset weapon upgrades
    m_Upgrades.magSizeModifier = 0;
    m_Upgrades.ammoCapModifier = 0;
    m_Upgrades.reloadTimeModifier = 1.0f;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0;
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
		m_currentWeapon->ammoContainer->fillMag(m_Upgrades.magSizeModifier);
		m_reloadState = ReloadingWeapon::IDLE;
		printf("adding ammo\n");
		printf("ammo: %d\n", m_currentWeapon->ammoContainer->getAmmoInfo().enhancedAmmo);
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

void WeaponManager::affect(Effect const & effect)
{
    long long flags = effect.getStandards()->flags;


    if (flags & Effect::EFFECT_MODIFY_AMMO)
    { 
        WeaponLoadout* wp = nullptr;
        if (effect.getSpecifics()->ammoType == 0)
            wp = getWeaponLoadout(0);
        else if (effect.getSpecifics()->ammoType == 1)
            wp = getWeaponLoadout(1);

        if (wp)
        {
            // Only add base magsize because we removed ammo
            int magSize = wp->ammoContainer->getAmmoInfo().magSize;
            int ammoCap = wp->ammoContainer->getAmmoInfo().enhancedAmmoCap + m_Upgrades.ammoCapModifier;
            int currentAmmo = wp->ammoContainer->getAmmoInfo().enhancedAmmo;

            if ((currentAmmo + magSize) >= ammoCap)
                wp->ammoContainer->setEnhancedAmmo(ammoCap);
            else
                wp->ammoContainer->setEnhancedAmmo(currentAmmo + magSize);
        }
    }
}

void WeaponManager::onUpgradeAdd(int stacks, Upgrade const & upgrade)
{
    long long flags = upgrade.getTranferEffects();

    if (flags & Upgrade::UPGRADE_DECREASE_RELOADTIME)
    {
        m_Upgrades.reloadTimeModifier *= 1 - upgrade.getFlatUpgrades().increaseCooldown;
        if (m_Upgrades.reloadTimeModifier < 0)
        {
            m_Upgrades.reloadTimeModifier = 0;
        }
    }
    if (flags & Upgrade::UPGRADE_INCREASE_FIRERATE)
    {
        m_Upgrades.fireRateModifier *= 1 - upgrade.getFlatUpgrades().increaseCooldown;
    }
    if (flags & Upgrade::UPGRADE_INCREASE_MAGSIZE)
    {
        m_Upgrades.magSizeModifier += upgrade.getFlatUpgrades().increaseMagSize;
    }
    if (flags & Upgrade::UPGRADE_INCREASE_AMMOCAP)
    {
        m_Upgrades.ammoCapModifier += upgrade.getFlatUpgrades().increaseAmmoCap;
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
        /* Primary */       newd WeaponCrossbow(projectileManager, ProjectileData("Icecone", false, true, 25, 1.f, 1, 100, 0.f, 3000, Resources::Models::Crossbowbolt , 1), Weapon::WeaponInfo{ 0, 1, 0, 0, 300, 0, 0,{ -0.5f, -0.5f, 0.f } }),
        /* Secondary*/      newd WeaponCrossbow(projectileManager, ProjectileData("Icecone", false, true, 25, 1.f, 1, 100, 0.f, 1000, Resources::Models::Crossbowbolt, 1), Weapon::WeaponInfo{ 1, 18, 15, 4, 100, 0, 0,{ 0.f, 0.f, 0.f } }),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 90, 0, 30, 30, { 1, 5 }, { 1, 5 }, 1000 }),
        /* WeaponModel */   newd WeaponModel(Resources::Models::Crossbow, WeaponModel::WeaponModelAnimationInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.f, 0.f, 0.f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.8f, -0.8f, 0.3f)), DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 800.f }) },

        // Ice
        newd WeaponLoadout{
        /* Primary */       newd Weapon(projectileManager, ProjectileData("Icecone", true, false, 0, 1, 1, 20, 0, 675, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeIce, true, false, false), Weapon::WeaponInfo{ 2, 1, 17, 5, 750, 0, 1,{ -0.6f, 0.25f, -1.3f } }),
        /* Secondary*/      newd WeaponFreezeGrenade(projectileManager, ProjectileData(nullptr, false, false, 40, 1.f, 1, 100, 0, 5000, Resources::Models::Ammocrystal, 1, ProjectileType::ProjectileTypeFreezeGrenade), Weapon::WeaponInfo{ 3, 1, 0, 0, 50, 0, 0,{ -0.8f, 0.2f, 0.f } }, ProjectileData(nullptr, false, false, 25, 0.5f, 1, 10, 5, 5000, Resources::Models::Ammocrystal, 1, ProjectileType::ProjectileTypeIceShard), 8),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 300, 0, 100, 100, { 1, 0 }, { 0, 25 }, 1500 }),
        /* WeaponModel */   newd WeaponModel(Resources::Models::Staff, WeaponModel::WeaponModelAnimationInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.f, -0.2f, 0.f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.8f, -2.3f, 0.3f)), DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 800.f }) },

        // Sledge/Melee
        newd WeaponLoadout{
        /* Primary */       newd Weapon(projectileManager, ProjectileData(nullptr, false, false, 35, 8.f, 1, 0, 0, 0, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMelee, true, false, false), Weapon::WeaponInfo{ 4, 1, 0, 0, 400, 200, 0,{ 0.f, 0.f, 0.f } }),
        /* Secondary*/      newd WeaponMeleeParry(projectileManager, ProjectileData(nullptr, false, false, 0, 8.f, 1, 0, 0, 200, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMeleeParry, true, false, true), Weapon::WeaponInfo{ 5, 1, 0, 0, 50, 0, 0,{ 0.f, 0.f, 0.f } }, 8.f),
        /* AmmoContainer */ newd AmmoContainer(AmmoContainer::AmmoInfo{ 0, 0, 0, 0, { 0, 0 }, { 0, 0 }, 0 }),
        /* WeaponModel */   newd WeaponModel(Resources::Models::Hammer, WeaponModel::WeaponModelAnimationInfo{ DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(1.3f, 0.9f, 0.f), DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-0.3f, -1.5f, -0.2f)), DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 200.f }) }
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
        if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().ammoConsumption[WEAPON_PRIMARY] == 0)
        {
            if (int delayTime = m_currentWeapon->primary->getDelayTime())
            {
                m_toUse = USE_PRIMARY;
                m_toUseShooter = &shooter;
                m_attackRateTimer = (float)delayTime;
            }
            else
            {
                Entity* shooterEntity = &shooter;
                usePrimary(position, yaw, pitch, *shooterEntity);

                Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_CUTLERY_PRIMARY, nullptr, true);
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
    if (m_currentWeapon->primary->useEnhanced(m_currentWeapon->ammoContainer->removeAmmo(WEAPON_PRIMARY)))
    {
        m_currentWeapon->primary->use(position, yaw, pitch, shooter);
        m_attackRateTimer = m_currentWeapon->primary->getAttackTimer(m_Upgrades.fireRateModifier);
    }
}

void WeaponManager::tryUseSecondary(btVector3 position, float yaw, float pitch, Player& shooter)
{
    if (m_attackRateTimer <= 0.f)
    {
        if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().ammoConsumption[WEAPON_SECONDARY] == 0)
        {
            if (int delayTime = m_currentWeapon->secondary->getDelayTime())
            {
                m_toUse = USE_SECONDARY;
                m_toUseShooter = &shooter;
                m_attackRateTimer = (float)delayTime;
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
    if (m_currentWeapon->secondary->useEnhanced(m_currentWeapon->ammoContainer->removeAmmo(WEAPON_SECONDARY)))
    {
        m_currentWeapon->secondary->use(position, yaw, pitch, shooter);
        m_attackRateTimer = m_currentWeapon->secondary->getAttackTimer(m_Upgrades.fireRateModifier);
    }
}

void WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f &&
        m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo < (m_currentWeapon->ammoContainer->getAmmoInfo().magSize + m_Upgrades.magSizeModifier))
	{
        m_reloadTimer = m_currentWeapon->ammoContainer->getAmmoInfo().reloadTime * m_Upgrades.reloadTimeModifier;
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
