#include "Player/Skill/SkillShieldCharge.h"

using namespace Logic;

SkillShieldCharge::SkillShieldCharge(ProjectileManager * projectileManager, ProjectileData projectileData)
: Skill(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION)
{
	m_projectileManager = projectileManager;
	m_projectileData = projectileData;
	m_active = false;
	m_time = 0.0f;
	m_forw = btVector3(0.0f, 0.0f, 0.0f);
	m_thePlayer = nullptr;
	m_chargePower = 10000.0f;
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
		m_forw = m_forw * m_chargePower;
		m_thePlayer = &shooter;
		m_thePlayer->getRigidbody()->applyCentralImpulse(m_forw);
	}
}

void SkillShieldCharge::onUpdate(float deltaTime)
{
	if (m_active == true)
	{
		if (m_time >= SHIELD_CHARGE_DURATION)
		{
			btVector3 impulse = (-m_thePlayer->getRigidbody()->getLinearVelocity()) / m_thePlayer->getRigidbody()->getLinearFactor() / m_thePlayer->getRigidbody()->getInvMass();
			m_thePlayer->getRigidbody()->applyCentralImpulse(impulse);
			m_active = false;
			m_time = 0;
		}
		m_time += deltaTime;
	}
}

void SkillShieldCharge::render(Graphics::Renderer& renderer)
{

}