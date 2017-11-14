#include "Player/Skill/Skill.h"

using namespace Logic;

Skill::Skill(float cooldown, float duration)
{
	m_cooldownMax = cooldown;
	m_cooldown = -1.f;
    m_cooldownModifer = 1.0f;
	m_durationMax = duration;
	m_duration = -1.f;
	m_canUse = true;
    m_active = false;
}

Skill::~Skill() { }

void Skill::use(btVector3 forward, Entity& shooter)
{
	if (m_canUse)
	{
        if (onUse(forward, shooter))
        {
            m_duration = m_durationMax;
            m_active = true;
            m_cooldown = m_cooldownMax * m_cooldownModifer;

        }
	}
}

void Skill::release()
{
	if (m_active)
	{
		// Specific release stuff
        m_canUse = false;
        m_active = false;
		onRelease();
	}
}

void Skill::update(float deltaTime)
{
    // Reloading
    if (m_cooldown <= 0 && !m_active)
    {
        m_canUse = true;
        m_cooldown = 0;
    }else 
		m_cooldown -= deltaTime;

	if (m_duration > 0)
		m_duration -= deltaTime;

	onUpdate(deltaTime);
}

void Skill::affect(Effect const & effect)
{
    long long flags = effect.getStandards()->flags;
    if (flags & Effect::EFFECT_DECREASE_CD)
    {
        m_cooldownModifer -= m_cooldownModifer * effect.getModifiers()->modifySkillCDDecrease;
        if (m_cooldownModifer < 0)
        {
            m_cooldownModifer = 0;
        }
    }
    else
    {
        onAffect(effect);
    }
}

float Skill::getCooldown() const		        { return m_cooldown;			}
float Skill::getCooldownMax() const		        { return m_cooldownMax;			}
float Skill::getDuration() const		        { return m_duration;			}
float Skill::getDurationMax() const		        { return m_durationMax;			}
bool Skill::getCanUse() const			        { return m_canUse;				}
void Skill::setCooldown(float cooldown)         { this->m_cooldown = cooldown;  }
void Skill::setCooldownMax(float cooldownMax)   { this->m_cooldownMax = cooldownMax; }
void Skill::setCanUse(bool canUse)		        { this->m_canUse = canUse;		}
