#include "Player/Skill/SkillShieldCharge.h"

using namespace Logic;

SkillShieldCharge::SkillShieldCharge(float cooldown, float duration, ProjectileManager * projectileManager, ProjectileData projectileData)
: Skill(cooldown, duration)
{
	m_projectileManager = projectileManager;
	m_projectileData = projectileData;
}

SkillShieldCharge::~SkillShieldCharge()
{
	m_projectileManager = nullptr;
}

void SkillShieldCharge::onUse(btVector3 forward, Entity& shooter)
{
	printf("Used Shield Charge.\n");
}

void SkillShieldCharge::onUpdate(float deltaTime)
{

}

void SkillShieldCharge::render(Graphics::Renderer& renderer)
{

}