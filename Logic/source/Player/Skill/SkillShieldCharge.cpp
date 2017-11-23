#include "Player/Skill/SkillShieldCharge.h"
#include <Player\Player.h>
#include <Engine\Settings.h>
#include <Graphics\include\MainCamera.h>

#define SHIELD_CHARGE_CD                        5000.f
#define SLOWDOWN_DURATION                       150.f
#define SHIELD_CHARGE_DURATION                  400.f
#define SHIELD_CHARGE_FOV_INCREASE_SPEED        0.002f
#define SHIELD_CHARGE_FOV_DECREASE_SPEED        0.014f
#define SHIELD_CHARGE_FOV_MULTIPLIER_TARGET     1.5f
#define SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT    1.f

using namespace Logic;

SkillShieldCharge::SkillShieldCharge()
: Skill(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION)
{
	m_active = false;
	m_slowdown = false;

	m_time = 0.0f;
	m_forw = btVector3(0.0f, 0.0f, 0.0f);
	m_chargePower = 5.f;
    m_fovM = SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT;
}

SkillShieldCharge::~SkillShieldCharge()
{

}

bool SkillShieldCharge::onUse(btVector3 forward, Entity& shooter)
{
    setCanUse(false);

	if (!m_active)
	{
        shooter.getSoundSource()->playSFX(Sound::SFX::SKILL_CHARGE);

		//Sets up the shield charge by deciding its movement vector and who is gonna charge
		m_time = 0;

		m_active = true;
		m_slowdown = false;

		m_forw = forward;
		m_forw.setY(0);
		m_forw = m_forw.normalize();
		m_shooter = &shooter;
		shooter.getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::SHIELD_CHARGE, 1);
		
		if (Player* player = dynamic_cast<Player*>(m_shooter))
			player->setMaxSpeed(m_chargePower * PLAYER_MOVEMENT_MAX_SPEED);

        return true;
	}

    return false;
}

void SkillShieldCharge::onRelease() { }

void SkillShieldCharge::onUpdate(float deltaTime)
{
	if (m_active)
	{
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			player->setPlayerState(Player::PlayerState::IN_AIR);

			//Pushes the player forward with a static charge power, charging towards the pre decided vector
			player->setMoveSpeed(m_chargePower * PLAYER_MOVEMENT_MAX_SPEED);
			player->setMoveDirection(m_forw);

            // FOV increase effect
            m_fovM += (SHIELD_CHARGE_FOV_MULTIPLIER_TARGET - m_fovM) * SHIELD_CHARGE_FOV_INCREASE_SPEED * deltaTime;
            Global::mainCamera->updateFOV(m_fovM * Settings::getInstance().getFOV());

			m_time += deltaTime;

			if (m_time >= SHIELD_CHARGE_DURATION)
			{
				// When the duration of the skill is up the current charge is put to the max character speed
				m_slowdown = true;
				m_active = false;
				m_time = 0;
			}
		}
	} 
	else if (m_slowdown)
	{
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			m_time += deltaTime;
			float power = m_chargePower * ((SLOWDOWN_DURATION - m_time) / SLOWDOWN_DURATION);
			player->setMoveSpeed(power * PLAYER_MOVEMENT_MAX_SPEED);

            // FOV effect
            m_fovM += (SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT - m_fovM) * SHIELD_CHARGE_FOV_DECREASE_SPEED * deltaTime;
            Global::mainCamera->updateFOV(m_fovM * Settings::getInstance().getFOV());

			if (m_time >= SLOWDOWN_DURATION)
			{
				player->setMaxSpeed(PLAYER_MOVEMENT_MAX_SPEED);
				player->setMoveSpeed(PLAYER_MOVEMENT_MAX_SPEED);
				m_slowdown = false;
                m_fovM = SHIELD_CHARGE_FOV_MULTIPLIER_DEFAULT;
                Global::mainCamera->updateFOV(Settings::getInstance().getFOV());
			}
		}
	}
}

void Logic::SkillShieldCharge::onUpgradeAdd(int stacks, Upgrade const & upgrade)
{
}

void SkillShieldCharge::render() const
{

}
