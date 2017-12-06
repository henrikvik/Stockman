#include "Player/Weapon/WeaponManager.h"
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\WeaponCrossbow.h>
#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Player\Weapon\WeaponFreezeGun.h>
#include <Player\Weapon\WeaponMeleeParry.h>
#include <Player\Weapon\WeaponMeleeSwing.h>
#include <Player\Weapon\WeaponAnimation.h>
#include <Misc\Sound\NoiseMachine.h>

#include <Player\Player.h>

#include <Graphics\include\Structs.h>
#include <Projectile\ProjectileStruct.h>

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { clear(); }

void WeaponManager::init(ProjectileManager* projectileManager)
{
	initializeWeapons(projectileManager);
	m_currentWeapon = &m_weaponLoadouts[0];

	// Timers
	m_attackRateTimer = -1.f;
    m_toUse = USE_NOTHING;
    m_toUseShooter = nullptr;

	m_reloadTimer = 0.f;
	m_reloadState = ReloadingWeapon::IDLE;

    m_Upgrades.magSizeModifier = 1.f;
    m_Upgrades.ammoCapModifier = 0;
    m_Upgrades.reloadTimeModifier = 1.0f;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0;

    m_ammoPickedUp = 0;
}

void WeaponManager::clear()
{
    for (WeaponLoadout& weaponLoadout : m_weaponLoadouts)
    {
        delete weaponLoadout.weapon[WEAPON_PRIMARY];
        delete weaponLoadout.weapon[WEAPON_SECONDARY];
        delete weaponLoadout.weaponModel;
    }
        
	m_weaponLoadouts.clear();
}

void WeaponManager::reset()
{
    // Reset ammo
	for (int i = 0; i < m_weaponLoadouts.size(); i++)
        m_weaponLoadouts[i].ammoContainer.reset();
    

    // Reset current weapon
    m_currentWeapon = &m_weaponLoadouts[0];

    m_attackRateTimer = 0;
    m_toUse = USE_NOTHING;
    m_toUseShooter = nullptr;

    m_reloadTimer = 0.f;
    m_reloadState = ReloadingWeapon::IDLE;

    //reset weapon upgrades
    m_Upgrades.magSizeModifier = 1.f;
    m_Upgrades.ammoCapModifier = 0;
    m_Upgrades.reloadTimeModifier = 1.0f;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0;
}

void WeaponManager::setWeaponModel(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    // Updating weapon model
    m_currentWeapon->weaponModel->update(deltaTime, playerTranslation, playerForward);
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
		m_currentWeapon->ammoContainer.fillMag(m_Upgrades.magSizeModifier);
		m_reloadState = ReloadingWeapon::IDLE;
	}

	// Attack
    if (m_attackRateTimer > 0.f)
    {
        m_attackRateTimer -= deltaTime;

        if (m_attackRateTimer < FLT_EPSILON && m_toUse != USE_NOTHING)
        {
            Entity* shooterEntity = m_toUseShooter;

            if (m_toUse == USE_PRIMARY)
                attack(WEAPON_PRIMARY, m_toUseShooter->getPositionBT() + m_toUseShooter->getForwardBT(), m_toUseShooter->getYaw(), m_toUseShooter->getPitch(), *shooterEntity);
            else
                attack(WEAPON_SECONDARY, m_toUseShooter->getPositionBT() + m_toUseShooter->getForwardBT(), m_toUseShooter->getYaw(), m_toUseShooter->getPitch(), *shooterEntity);

            m_toUseShooter = nullptr;
            m_toUse = USE_NOTHING;
        }
    }

    m_currentWeapon->weaponModel->setIsEnhanced(m_currentWeapon->ammoContainer.getIsEnhanced());
}

void WeaponManager::affect(Effect const & effect)
{
    long long flags = effect.getStandards()->flags;

    if (flags & Effect::EFFECT_MODIFY_AMMO)
    { 
        WeaponLoadout* wp = nullptr;
        if (effect.getSpecifics()->ammoType == 0)
        {
            wp = getWeaponLoadout(0);
            m_ammoPickedUp = 1;
        }
        else if (effect.getSpecifics()->ammoType == 1)
        {
            wp = getWeaponLoadout(1);
            m_ammoPickedUp = 2;
        }

        if (wp)
        {
            // Only add base magsize because we removed ammo
            int magSize = wp->ammoContainer.getAmmoInfo().magSize;
            int ammoCap = wp->ammoContainer.getAmmoInfo().enhancedAmmoCap + m_Upgrades.ammoCapModifier;
            int currentAmmo = wp->ammoContainer.getAmmoInfo().enhancedAmmo;

            if ((currentAmmo + magSize) >= ammoCap)
                wp->ammoContainer.setEnhancedAmmo(ammoCap);
            else
                wp->ammoContainer.setEnhancedAmmo(currentAmmo + magSize);
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
    m_weaponLoadouts.reserve(3);
    WeaponLoadout wl;

    // ############
    // **Crossbow**
    // ############
    /* Primary */       wl.weapon[0] = newd WeaponCrossbow(projectileManager,
                                    ProjectileData("Icecone", false, true, 25.f, 1.5f, 1.f, 90.f, 0.1f, 3000.f, LightRenderInfo(DirectX::SimpleMath::Color(1.f, 0.8f, 1.f, 1.f), .25f, 3.f), Resources::Models::Crossbowbolt, 1, ProjectileTypeNormal, true, false, true, false, { 0.f, -0.4f, 0.f }),
                                    Weapon::WeaponInfo{ 0, 1, 0, 0, 300, 0, 0, { -0.5f, -0.5f, 0.f }, 800.f }, 
                                    25.f, 35.f);
    /* Secondary */     wl.weapon[1] = newd WeaponCrossbow(projectileManager,
                                    ProjectileData("Icecone", false, true, 20.f, 1.5f, 1.f, 70.f, 0.1f, 1500.f, LightRenderInfo(DirectX::SimpleMath::Color(0.9f, 0.5f, 1.f, 1.f), .25f, 1.5f), Resources::Models::Crossbowbolt, 1, ProjectileType::ProjectileTypeNormal, true, false, true, true, { 0.f, -0.4f, 0.f }),
                                    Weapon::WeaponInfo{ 1, 18, 15, 4, 50, 0, 0, { -0.5f, -0.5f, 0.f }, 800.f }, 
                                    25.f, 35.f);
    /* Ammo */          wl.ammoContainer = AmmoContainer(AmmoContainer::AmmoInfo{ 999, 0, 30, 30,{ 1, 10 },{ 1, 10 }, 1000 });
    /* WeaponModel */   wl.weaponModel = newd WeaponAnimation(Resources::Models::Crossbow, ModelAnimation::AnimationFrame{
        /* Model rotation */        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.f, 0.f, 0.f),
        /* Model position */        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.8f, -0.8f, 0.3f)),
        /* Model scale */           DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f) , 1.f });
    m_weaponLoadouts.push_back(wl);

    // #######
    // **Ice**
    // #######
    /* Primary */       wl.weapon[0] = newd WeaponFreezeGun(projectileManager,
                                    ProjectileData("Icecone", true, false, 0, 3.f, 1.f, 30.f, 0, 675.f, LightRenderInfo(DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 0.05f), .05f, 8.f), Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeIce, true, false, false),
                                    Weapon::WeaponInfo{ 2, 1, 17, 5, 750, 0, 1,{ -0.6f, 0.25f, -1.3f }, 800.f });
    /* Secondary */     wl.weapon[1] = newd WeaponFreezeGrenade(projectileManager,
                                    ProjectileData("IceTrail", false, true, 0, 1.f, 1.f, 50.f, 5.f, 5000.f, LightRenderInfo(DirectX::SimpleMath::Color(0.15f, 0.4f, 1.f, 1.f), 0.10f, 3.f), Resources::Models::Ammocrystal, 1, ProjectileType::ProjectileTypeFreezeGrenade, false, false, false),
                                    Weapon::WeaponInfo{ 3, 1, 0, 0, 30, 0, 0,{ -0.8f, 0.2f, 0.f }, 800.f },
                                    ProjectileData("IceTrail", false, true, 0, 1.f, 1.f, 10.f, 5.f, 5000.f, LightRenderInfo(DirectX::SimpleMath::Color(0.15f, 0.4f, 1.f, 1.f), 0.01f, 1.f), Resources::Models::Ammocrystal, 1, ProjectileType::ProjectileTypeIceShard, false, false, false),
                                    ProjectileData(nullptr, false, false, 50, 1.f, 1.f, 0, 0, 0, LightRenderInfo(DirectX::SimpleMath::Color(0.15f, 0.4f, 1.f, 1.f), 0.01f, 1.f), Resources::Models::UnitCube, 1, ProjectileTypeFreezeExplosion, true, false, false),
                                    8);
    /* Ammo */          wl.ammoContainer = AmmoContainer(AmmoContainer::AmmoInfo{ 999, 0, 100, 100, { 1, 25 }, { 0, 25 }, 1500 });
    /* WeaponModel */   wl.weaponModel = newd WeaponFreezeGunAnimation(Resources::Models::Staff, ModelAnimation::AnimationFrame{
        /* Model rotation */        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.f, -0.1f, 0.015f),
        /* Model position */        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.7f, -2.2f, 1.3f)),
        /* Model scale */           DirectX::SimpleMath::Matrix::CreateScale(0.5f, 0.5f, 0.5f), 1.f });
    m_weaponLoadouts.push_back(wl);

    // #########
    // **Melee**
    // #########
    /* Primary */       wl.weapon[0] = newd WeaponMeleeSwing(projectileManager,
                                    ProjectileData("Icecone", true, false, 100, 30.f, 1.f, 0, 0, 0, LightRenderInfo(DirectX::SimpleMath::Color(0.85f, 0.3f, 0.4f, 1.f), 0.85f, 10.f), Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMelee, true, false, false),
                                    Weapon::WeaponInfo{ 4, 1, 0, 0, 90, 225, 0,{ 0.f, 0.f, 0.f }, 0.f }, 10.f);
    /* Secondary */     wl.weapon[1] = newd WeaponMeleeParry(projectileManager,
                                    ProjectileData(nullptr, false, false, 0, 30.f, 1.f, 0, 0, 0, LightRenderInfo(DirectX::SimpleMath::Color(0.15f, 0.8f, 0.6f, 1.f), 0.85f, 15.f), Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMeleeParry, true, false, false),
                                    Weapon::WeaponInfo{ 5, 1, 0, 0, 100, 0, 0,{ 0.f, 0.f, 0.f }, 0.f }, 22.5f);
    /* Ammo */          wl.ammoContainer = AmmoContainer(AmmoContainer::AmmoInfo{ 0, 0, 0, 0, { 0, 0 }, { 0, 0 }, 0 });
    /* WeaponModel */   wl.weaponModel = newd WeaponSledgeHammerAnimation(Resources::Models::Hammer, ModelAnimation::AnimationFrame{
        /* Model rotation */        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(1.3f, 0.9f, 0.f),
        /* Model position */        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-0.3f, -1.5f, -0.2f)),
        /* Model scale */           DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 1.f });
    m_weaponLoadouts.push_back(wl);
}

void WeaponManager::switchWeapon(int index)
{
	// Check if swapTimer is active or swapping to same weapon 
	if (!(m_currentWeapon == &m_weaponLoadouts[index]))
	{
		m_currentWeapon = &m_weaponLoadouts[index];
		m_attackRateTimer = m_currentWeapon->weapon[0]->getSwapTimer();
        m_toUse = USE_NOTHING;
        m_toUseShooter = nullptr;

		m_reloadTimer = 0.f;
		m_reloadState = ReloadingWeapon::IDLE;
        
        m_currentWeapon->weaponModel->startSwapToAnimation(m_currentWeapon->weapon[0]->getSwapTimer());
	}
}

void WeaponManager::tryAttack(int attackMode, btVector3 position, float yaw, float pitch, Player& shooter)
{
    if (m_attackRateTimer <= 0.f)
    {
        if (m_currentWeapon->ammoContainer.getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer.getAmmoInfo().ammoConsumption[attackMode] == 0)
        {
            if (int delayTime = m_currentWeapon->weapon[attackMode]->getDelayTime())
            {
                m_toUse = WeaponToUse(attackMode);
                m_toUseShooter = &shooter;
                m_attackRateTimer = (float)delayTime * m_Upgrades.fireRateModifier;;
                m_currentWeapon->weaponModel->startWindupAnimation(delayTime);
            }
            else
            {
                Entity* shooterEntity = &shooter;
                attack(attackMode, position, yaw, pitch, *shooterEntity);
            }
        }
        else
        {
            reloadWeapon();
        }
    }
}

void WeaponManager::attack(int attackMode, btVector3 position, float yaw, float pitch, Entity& shooter)
{
    bool primary = false;
    if (attackMode == WeaponToUse::USE_PRIMARY) primary = true;
    m_currentWeapon->weaponModel->startShootAnimation(m_currentWeapon->weapon[attackMode]->getAttackTimer(m_Upgrades.fireRateModifier), primary);
    m_currentWeapon->weapon[attackMode]->useEnhanced(m_currentWeapon->ammoContainer.removeAmmo(attackMode));
    m_currentWeapon->weapon[attackMode]->use(position, yaw, pitch, shooter);
    m_attackRateTimer = m_currentWeapon->weapon[attackMode]->getAttackTimer(m_Upgrades.fireRateModifier);
}

void WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f &&
        m_currentWeapon->ammoContainer.getAmmoInfo().magAmmo < static_cast<int>(m_currentWeapon->ammoContainer.getAmmoInfo().magSize * m_Upgrades.magSizeModifier))
	{
        Sound::NoiseMachine::Get().playSFX(Sound::WEAPON_RELOAD, nullptr, true);
        m_currentWeapon->weaponModel->startReloadingAnimation(m_currentWeapon->ammoContainer.getAmmoInfo().reloadTime * m_Upgrades.reloadTimeModifier);
        m_reloadTimer = m_currentWeapon->ammoContainer.getAmmoInfo().reloadTime * m_Upgrades.reloadTimeModifier;
		m_reloadState = ReloadingWeapon::ACTIVE;
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
    return &m_weaponLoadouts[index];
}

WeaponManager::WeaponLoadout* WeaponManager::getActiveWeaponLoadout()
{
    if (&m_weaponLoadouts[0] == m_currentWeapon)
    {
        return &m_weaponLoadouts[0];
    }
    else
    {
        return &m_weaponLoadouts[1];
    }
}

WeaponManager::WeaponLoadout* WeaponManager::getInactiveWeaponLoadout()
{
    if (&m_weaponLoadouts[1] == m_currentWeapon || &m_weaponLoadouts[2] == m_currentWeapon)
    {
        return &m_weaponLoadouts[0];
    }
    else
    {
        return &m_weaponLoadouts[1];
    };
}

int Logic::WeaponManager::getAmmoPickedUp()
{
    int temp = m_ammoPickedUp;
    m_ammoPickedUp = 0;
    return temp;
}
