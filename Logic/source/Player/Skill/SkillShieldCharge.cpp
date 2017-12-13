#include "Player/Skill/SkillShieldCharge.h"
#include <Player\Player.h>
#include <Engine\Settings.h>
#include <Graphics\include\MainCamera.h>
#include <Projectile\ProjectileManager.h>
#include <Projectile\Projectile.h>
#include <Projectile\ProjectileStruct.h>
#include <AI\Enemy.h>

#define SHIELD_CHARGE_CD                        6000.f
#define SLOWDOWN_DURATION                       150.f
#define SHIELD_CHARGE_DURATION                  500.f
#define SHIELD_CHARGE_FOV_INCREASE_SPEED        0.002f
#define SHIELD_CHARGE_FOV_DECREASE_SPEED        0.014f
#define SHIELD_CHARGE_FOV_MULTIPLIER_TARGET     1.5f
#define SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT    1.f
#define SHIELD_CHARGE_KNOCKBACK_POWER           30.f

using namespace Logic;

SkillShieldCharge::SkillShieldCharge(ProjectileManager* projectileManager, ProjectileData& pData)
: Skill(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION)
{
	m_slowdown = false;

	m_time = 0.0f;
	m_forw = btVector3(0.0f, 0.0f, 0.0f);
	m_chargePower = 5.f;
    m_fovM = SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT;
    m_oldSpeed = 0.f;
    m_pData = newd ProjectileData(pData);
    m_projectile = nullptr;

    setSpawnFunctions(*projectileManager);
}

SkillShieldCharge::~SkillShieldCharge()
{
    delete m_pData;
}

void SkillShieldCharge::setSpawnFunctions(ProjectileManager &projManager)
{
    SpawnProjectile = [&](ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter) -> Projectile* {
        return projManager.addProjectile(pData, position, forward, shooter);
    };
}

bool SkillShieldCharge::onUse(btVector3 forward, Entity& shooter)
{
    shooter.getSoundSource()->playSFX(Sound::SFX::SKILL_CHARGE);

    //Sets up the shield charge by deciding its movement vector and who is gonna charge
    m_time = 0;

    m_slowdown = false;

    m_forw = forward;
    m_forw.setY(0);
    m_forw = m_forw.normalize();
    m_shooter = &shooter;
    shooter.getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::INVULNERABLE, 1);

    // Spawning projectile
    m_projectile = SpawnProjectile(*m_pData, shooter.getPositionBT(), forward, shooter);
    if (m_projectile)
    {
        m_projectile->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {
            PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

            if (Enemy* enemy = dynamic_cast<Enemy*>(obj))
            {
                btVector3 knockbackDir = (obj->getPositionBT() - shooter.getPositionBT()).normalize() + btVector3(0, 1, 0);
                obj->getRigidBody()->setLinearVelocity(knockbackDir * SHIELD_CHARGE_KNOCKBACK_POWER);
                enemy->getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::STUN, 1);
            }
        });
    }

    if (Player* player = dynamic_cast<Player*>(m_shooter))
    {
        m_oldSpeed = player->getMoveSpeed();
        player->setMaxSpeed(m_chargePower * PLAYER_MOVEMENT_MAX_SPEED);

        SpecialEffectRenderInfo shake = {};
        shake.duration = SHIELD_CHARGE_DURATION * 0.001f;
        shake.direction = { 0.f, -1.f, 0.f };
        shake.radius = 20.0f;
        shake.type = SpecialEffectRenderInfo::screenShake;
        shake.affectEveryThing = false;
        QueueRender(shake);
    }

    return true;
}

void SkillShieldCharge::onRelease() { }

void SkillShieldCharge::onUpdate(float deltaTime)
{
	if (isActive())
	{
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			player->setPlayerState(Player::PlayerState::IN_AIR);

			//Pushes the player forward with a static charge power, charging towards the pre decided vector
			player->setMoveSpeed(m_chargePower * PLAYER_MOVEMENT_MAX_SPEED);
			player->setMoveDirection(m_forw);

            // Move Projectile
            m_projectile->getTransform().setOrigin(player->getPositionBT() + player->getForwardBT());

            // FOV increase effect
            m_fovM += (SHIELD_CHARGE_FOV_MULTIPLIER_TARGET - m_fovM) * SHIELD_CHARGE_FOV_INCREASE_SPEED * deltaTime;
            Global::mainCamera->updateFOV(m_fovM * Settings::getInstance().getFOV());

			m_time += deltaTime;

            SpecialEffectRenderInfo tint = {};
            tint.type = SpecialEffectRenderInfo::Tint;
            tint.color = DirectX::SimpleMath::Vector3(0.15, 0.30, 1.0);
            tint.progress = 1.f - m_time / (float)SHIELD_CHARGE_DURATION;
            QueueRender(tint);

			if (m_time >= SHIELD_CHARGE_DURATION)
			{
				// When the duration of the skill is up the current charge is put to the max character speed
                setActive(false);
				m_slowdown = true;
				m_time = 0;
                m_projectile->setDead(true);
                m_projectile = nullptr;
			}
		}
	} 
	else if (m_slowdown)
	{
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			m_time += deltaTime;
			float power = m_chargePower * ((SLOWDOWN_DURATION - m_time) / SLOWDOWN_DURATION);
			player->setMoveSpeed(power * m_oldSpeed);

            // FOV effect
            m_fovM += (SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT - m_fovM) * SHIELD_CHARGE_FOV_DECREASE_SPEED * deltaTime;
            Global::mainCamera->updateFOV(m_fovM * Settings::getInstance().getFOV());

			if (m_time >= SLOWDOWN_DURATION)
			{
				player->setMaxSpeed(PLAYER_MOVEMENT_MAX_SPEED);
				player->setMoveSpeed(m_oldSpeed);
				m_slowdown = false;
                m_fovM = SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT;
                Global::mainCamera->updateFOV(Settings::getInstance().getFOV());
			}
		}
	}
}

void SkillShieldCharge::onUpgradeAdd(int stacks, Upgrade const & upgrade)
{
}

void SkillShieldCharge::render() const
{

}
