#include "Player/Skill/Skill.h"

using namespace Logic;

Skill::Skill(float cooldown, float duration)
{
	m_cooldownMax = cooldown;
	m_cooldown = -1.f;
	m_duration = duration;
	m_canUse = true;
}

void Skill::use(btVector3 forward, Entity& shooter)
{
	if (m_canUse)
	{
		onUse(forward, shooter);

		// Reset cooldown
		m_cooldown = m_cooldownMax;
		m_canUse = false;
	}
}

void Skill::update(float deltaTime)
{
	// Reloading
	if (m_cooldown < 0)
		m_canUse = true;
	else 
		m_cooldown -= deltaTime;

	onUpdate(deltaTime);
}

float Skill::getCooldown() const	{ return m_cooldown;	}
float Skill::getCooldownMax() const { return m_cooldownMax; }
float Skill::getDuration() const	{ return m_duration;	}
bool Skill::getCanUse() const		{ return m_canUse;		}
