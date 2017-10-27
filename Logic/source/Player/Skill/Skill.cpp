#include "Player/Skill/Skill.h"

using namespace Logic;

Skill::Skill(float cooldown, float duration)
{
	m_cooldownMax = cooldown;
	m_cooldown = -1.f;
	m_durationMax = duration;
	m_duration = -1.f;
	m_canUse = true;
}

Skill::~Skill() { }

void Skill::use(btVector3 forward, Entity& shooter)
{
	if (m_canUse)
	{
        if (onUse(forward, shooter))
        {
            m_duration = m_durationMax;
            m_cooldown = m_cooldownMax;
            m_canUse = false;
        }
	}
}

void Skill::release()
{
	if (m_canUse)
	{
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

	if (m_duration > 0)
		m_duration -= deltaTime;

	onUpdate(deltaTime);
}

float Skill::getCooldown() const		{ return m_cooldown;			}
float Skill::getCooldownMax() const		{ return m_cooldownMax;			}
float Skill::getDuration() const		{ return m_duration;			}
float Skill::getDurationMax() const		{ return m_durationMax;			}
bool Skill::getCanUse() const			{ return m_canUse;				}
void Skill::setCooldown(float cooldown) { this->m_cooldown = cooldown;  }
void Skill::setCanUse(bool canUse)		{ this->m_canUse = canUse;		}
