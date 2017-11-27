#include "Player/Weapon/WeaponManager.h"
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\WeaponCrossbow.h>
#include <Player\Weapon\WeaponFreezeGrenade.h>
#include <Player\Weapon\WeaponMeleeParry.h>
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

    m_Upgrades.magSizeModifier = 0;
    m_Upgrades.ammoCapModifier = 0;
    m_Upgrades.reloadTimeModifier = 1.0f;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0;
}

void WeaponManager::clear()
{
    for (WeaponLoadout& weaponLoadout : m_weaponLoadouts)
    {
        delete weaponLoadout.weapon[WEAPON_PRIMARY];
        delete weaponLoadout.weapon[WEAPON_SECONDARY];
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
    m_Upgrades.magSizeModifier = 0;
    m_Upgrades.ammoCapModifier = 0;
    m_Upgrades.reloadTimeModifier = 1.0f;
    m_Upgrades.fireRateModifier = 1.0f;
    m_Upgrades.freezeDurationModifier = 1.0f;
    m_Upgrades.fireDamageModifier = 0;
}

void WeaponManager::setWeaponModel(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
    // Updating weapon model
    m_currentWeapon->weaponModel.update(deltaTime, playerTranslation, playerForward);
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
		printf("adding ammo\n");
		printf("ammo: %d\n", m_currentWeapon->ammoContainer.getAmmoInfo().enhancedAmmo);
		printf("mag: %d\n", m_currentWeapon->ammoContainer.getAmmoInfo().magAmmo);
	}

	// Attack
    if (m_attackRateTimer > 0.f)
    {
        m_attackRateTimer -= deltaTime;

        if (m_attackRateTimer < 0.f && m_toUse != USE_NOTHING)
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
    m_currentWeapon->weaponModel.render();
}

void WeaponManager::initializeWeapons(ProjectileManager* projectileManager)
{
    m_weaponLoadouts.reserve(3);
    WeaponLoadout wl;

    // ############
    // **Crossbow**
    // ############
    /* Primary */       wl.weapon[0] = newd WeaponCrossbow(projectileManager,
                                    ProjectileData("Icecone", false, true, 25, 1.5f, 1, 90, 0.10f, 3000, LightRenderInfo(DirectX::SimpleMath::Color(1, 0.8, 1, 1), .25f, 3.f), Resources::Models::Crossbowbolt, 1),
                                    Weapon::WeaponInfo{ 0, 1, 0, 0, 300, 0, 0,{ -0.5f, -0.5f, 0.f } });
    /* Secondary */     wl.weapon[1] = newd WeaponCrossbow(projectileManager,
                                    ProjectileData("Icecone", false, true, 25, 1.5f, 1, 70, 0.10f, 2000, LightRenderInfo(DirectX::SimpleMath::Color(0.9, 0.5, 1, 1), .25f, 1.5f), Resources::Models::Crossbowbolt, 1, ProjectileType::ProjectileTypeNormal, false, false, true, true),
                                    Weapon::WeaponInfo{ 1, 18, 15, 4, 50, 0, 0,{ 0.f, 0.f, 0.f } });
    /* Ammo */          wl.ammoContainer = AmmoContainer(AmmoContainer::AmmoInfo{ 90, 0, 30, 30,{ 1, 5 },{ 1, 5 }, 1000 });
    /* WeaponModel */   wl.weaponModel = WeaponModel(Resources::Models::Crossbow, WeaponModel::WeaponModelAnimationInfo{
        /* Model rotation */        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.f, 0.f, 0.f),
        /* Model position */        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.8f, -0.8f, 0.3f)),
        /* Model scale */           DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 800.f });
    m_weaponLoadouts.push_back(wl);

    // #######
    // **Ice**
    // #######
    /* Primary */       wl.weapon[0] = newd Weapon(projectileManager,
                                    ProjectileData("Icecone", true, false, 0, 3, 1, 30, 0, 675, LightRenderInfo(DirectX::SimpleMath::Color(1, 1, 1, 0.05), .05f, 8.f), Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeIce, true, false, false),
                                    Weapon::WeaponInfo{ 2, 1, 17, 5, 750, 0, 1,{ -0.6f, 0.25f, -1.3f } });
    /* Secondary */     wl.weapon[1] = newd WeaponFreezeGrenade(projectileManager,
                                    ProjectileData("NecroProjTrail", false, true, 0, 1.f, 1, 50, 5.f, 5000, LightRenderInfo(DirectX::SimpleMath::Color(0.15, 0.4, 1, 1), 0.25f, 5.f), Resources::Models::Ammocrystal, 1, ProjectileType::ProjectileTypeFreezeGrenade, false, false, false),
                                    Weapon::WeaponInfo{ 3, 1, 0, 0, 50, 0, 0,{ -0.8f, 0.2f, 0.f } },
                                    ProjectileData("NecroProjTrail", false, true, 0, 1.f, 1, 10, 5.f, 5000, LightRenderInfo(DirectX::SimpleMath::Color(0.15, 0.4, 1, 1), 0.05f, 5.f), Resources::Models::Ammocrystal, 1, ProjectileType::ProjectileTypeIceShard, false, false, false),
                                    ProjectileData(nullptr, false, false, 100, 25, 1, 0, 0, 0, LightRenderInfo(DirectX::SimpleMath::Color(0.15, 0.4, 1, 1), 0.05f, 5.f), Resources::Models::UnitCube, 1, ProjectileTypeFreezeExplosion, true, false, false),
                                    8);
    /* Ammo */          wl.ammoContainer = AmmoContainer(AmmoContainer::AmmoInfo{ 300, 0, 100, 100,{ 1, 25 },{ 1, 25 }, 1500 });
    /* WeaponModel */   wl.weaponModel = WeaponModel(Resources::Models::Staff, WeaponModel::WeaponModelAnimationInfo{
        /* Model rotation */        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0.f, -0.2f, 0.f),
        /* Model position */        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.8f, -2.3f, 0.3f)),
        /* Model scale */           DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 800.f });
    m_weaponLoadouts.push_back(wl);

    // #########
    // **Melee**
    // #########
    /* Primary */       wl.weapon[0] = newd Weapon(projectileManager,
                                    ProjectileData(nullptr, false, false, 250, 15.f, 1, 0, 0, 100, LightRenderInfo(DirectX::SimpleMath::Color(0.85, 0.3, 0.4, 1), 0.85f, 10.f), Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMelee, true, false, false),
                                    Weapon::WeaponInfo{ 4, 1, 0, 0, 100, 200, 0,{ 0.f, 0.f, 0.f } });
    /* Secondary */     wl.weapon[1] = newd WeaponMeleeParry(projectileManager,
                                    ProjectileData(nullptr, false, false, 0, 12.5f, 1, 0, 0, 500, LightRenderInfo(DirectX::SimpleMath::Color(0.15, 0.8, 0.6, 1), 0.85f, 15.f), Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeMeleeParry, true, false, false),
                                    Weapon::WeaponInfo{ 5, 1, 0, 0, 50, 0, 0,{ 0.f, 0.f, 0.f } }, 8.f);
    /* Ammo */          wl.ammoContainer = AmmoContainer(AmmoContainer::AmmoInfo{ 0, 0, 0, 0,{ 0, 0 },{ 0, 0 }, 0 });
    /* WeaponModel */   wl.weaponModel = WeaponModel(Resources::Models::Hammer, WeaponModel::WeaponModelAnimationInfo{
        /* Model rotation */        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(1.3f, 0.9f, 0.f),
        /* Model position */        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-0.3f, -1.5f, -0.2f)),
        /* Model scale */           DirectX::SimpleMath::Matrix::CreateScale(1.f, 1.f, 1.f), 200.f });
    m_weaponLoadouts.push_back(wl);
}

void WeaponManager::switchWeapon(int index)
{
	// Check if swapTimer is active or swapping to same weapon 
	if (!(m_currentWeapon == &m_weaponLoadouts[index]))
	{
		m_currentWeapon = &m_weaponLoadouts[index];
		m_attackRateTimer = m_currentWeapon->weaponModel.getModelInfo().swapTimer;
        m_toUse = USE_NOTHING;
        m_toUseShooter = nullptr;

		m_reloadTimer = 0.f;
		m_reloadState = ReloadingWeapon::IDLE;

		printf("switch weapon %d\n", index);
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
                m_toUse = USE_PRIMARY;
                m_toUseShooter = &shooter;
                m_attackRateTimer = (float)delayTime;
            }
            else
            {
                Entity* shooterEntity = &shooter;
                attack(attackMode, position, yaw, pitch, *shooterEntity);

                Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_CUTLERY_PRIMARY, nullptr, true);
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
    if (m_currentWeapon->weapon[attackMode]->useEnhanced(m_currentWeapon->ammoContainer.removeAmmo(attackMode)))
    {
        m_currentWeapon->weapon[attackMode]->use(position, yaw, pitch, shooter);
        m_attackRateTimer = m_currentWeapon->weapon[attackMode]->getAttackTimer(m_Upgrades.fireRateModifier);
    }
}

void WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f &&
        m_currentWeapon->ammoContainer.getAmmoInfo().magAmmo < (m_currentWeapon->ammoContainer.getAmmoInfo().magSize + m_Upgrades.magSizeModifier))
	{
        m_reloadTimer = m_currentWeapon->ammoContainer.getAmmoInfo().reloadTime * m_Upgrades.reloadTimeModifier;
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
