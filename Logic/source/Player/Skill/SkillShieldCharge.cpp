#include "Player/Skill/SkillShieldCharge.h"
#include <Player\Player.h>
#include <iostream>

using namespace Logic;

SkillShieldCharge::SkillShieldCharge()
: Skill(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION)
{
	m_active = false;
	m_time = 0.0f;
	m_forw = btVector3(0.0f, 0.0f, 0.0f);
	m_thePlayer = nullptr;
	m_chargePower = 0.07f;

}

SkillShieldCharge::~SkillShieldCharge()
{
	m_projectileManager = nullptr;
	m_thePlayer = nullptr;
}

void SkillShieldCharge::onUse(btVector3 forward, Entity& shooter)
{
	if (!m_active)
	{

		printf("Used Shield Charge.\n");
		m_active = true;
		m_forw = btVector3(forward.getX(), 0.0f, forward.getZ());
		m_forw = m_forw;
		m_thePlayer = &shooter;
		shooter.getStatusManager().addStatus(StatusManager::EFFECT_ID::SHIELD_CHARGE, 1, true);
	}
}

void SkillShieldCharge::onRelease() { }

void SkillShieldCharge::onUpdate(float deltaTime)
{
	if (m_active)
	{
		if (Player* player = dynamic_cast<Player*>(m_thePlayer))
		{
			player->setMaxSpeed(m_chargePower);
			player->setMoveSpeed(m_chargePower);
			player->setMoveDirection(m_forw);
		if (m_time >= SHIELD_CHARGE_DURATION)
		{
			player->setMaxSpeed(PLAYER_MOVEMENT_MAX_SPEED);
			m_active = false;
			m_time = 0;
			player->setMoveSpeed(PLAYER_MOVEMENT_MAX_SPEED);
		}

		m_time += deltaTime;

		}
	}
}

void SkillShieldCharge::render(Graphics::Renderer& renderer)
{

}
