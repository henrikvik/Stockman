#include "Player/Skill/SkillGrapplingHook.h"

using namespace Logic;

SkillGrapplingHook::SkillGrapplingHook(float cooldown, ProjectileManager * projectileManager, ProjectileData projectileData)
: Skill(cooldown)
{
	m_projectileManager = projectileManager;
	m_projectileData = projectileData;
}

SkillGrapplingHook::~SkillGrapplingHook()
{
	m_projectileManager = nullptr;
}

void SkillGrapplingHook::onUse(btVector3 forward, Entity& shooter)
{
	printf("Used Grappling Hook.\n");
	m_projectileManager->addProjectile(m_projectileData, shooter.getPositionBT(), forward, shooter);
}

void SkillGrapplingHook::onUpdate(float deltaTime)
{

}

void SkillGrapplingHook::render(Graphics::Renderer& renderer)
{

}