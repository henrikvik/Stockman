#include "Player/Skill/SkillShieldCharge.h"
#include <Player\Player.h>

#define SHIELD_CHARGE_CD 5000.f
#define SLOWDOWN_DURATION 100.f
#define SHIELD_CHARGE_DURATION 400.f

using namespace Logic;

SkillShieldCharge::SkillShieldCharge()
: Skill(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION)
{
	m_active = false;
	m_slowdown = false;

	m_time = 0.0f;
	m_forw = btVector3(0.0f, 0.0f, 0.0f);
    m_chargePower = 5.f;
}

SkillShieldCharge::~SkillShieldCharge()
{

}

void SkillShieldCharge::onUse(btVector3 forward, Entity& shooter)
{
	if (!m_active)
	{
		//Sets up the shield charge by deciding its movement vector and who is gonna charge
		m_time = 0;

		m_active = true;
		m_slowdown = false;

		m_forw = forward;
		m_forw.setY(0);
		m_forw = m_forw.normalize();
		m_shooter = &shooter;
		shooter.getStatusManager().addStatus(StatusManager::EFFECT_ID::SHIELD_CHARGE, 1, true);
		
		if (Player* player = dynamic_cast<Player*>(m_shooter))
			player->setMaxSpeed(m_chargePower * PLAYER_MOVEMENT_MAX_SPEED);
	}
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
			if (m_time >= SLOWDOWN_DURATION)
			{
				player->setMaxSpeed(PLAYER_MOVEMENT_MAX_SPEED);
				player->setMoveSpeed(PLAYER_MOVEMENT_MAX_SPEED);
				m_slowdown = false;
			}
		}
	}
}

void SkillShieldCharge::render(Graphics::Renderer& renderer)
{

}
