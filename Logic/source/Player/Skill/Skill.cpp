#include "Player/Skill/Skill.h"

using namespace Logic;

Skill::Skill(float cooldown, float duration)
{
	m_cooldownMax = cooldown;
	m_cooldown = -1.f;
	m_duration = duration;
	m_canUse = true;
}

Skill::~Skill() { }

void Skill::use(btVector3 forward, Entity& shooter)
{
	if (m_canUse)
	{
		onUse(forward, shooter);
	}
}

void Skill::release()
{
	if (!m_canUse)
	{
		// Reset cooldown
		m_cooldown = m_cooldownMax;
		m_canUse = false;

		// Specific release stuff
		onRelease();
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

float Skill::getCooldown() const		{ return m_cooldown;			}
float Skill::getCooldownMax() const		{ return m_cooldownMax;			}
float Skill::getDuration() const		{ return m_duration;			}
bool Skill::getCanUse() const			{ return m_canUse;				}
void Skill::setCooldown(float cooldown) { this->m_cooldown = cooldown;  }
void Skill::setCanUse(bool canUse)		{ this->m_canUse = canUse;		}
