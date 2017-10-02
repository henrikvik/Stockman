#include "Player/Skill/SkillBulletTime.h"

using namespace Logic;

SkillBulletTime::SkillBulletTime(ProjectileManager* projectileManager, ProjectileData pData)
: Skill(BULLET_TIME_CD)
{
	m_projectileData = pData;
	m_projectileManager = projectileManager;
}

void SkillBulletTime::onUse(btVector3 forward, Entity& shooter)
{
	printf("Bullet Time used.\n");
}

void SkillBulletTime::onRelease() { }

void SkillBulletTime::onUpdate(float deltaTime)
{

}

void SkillBulletTime::render(Graphics::Renderer& renderer)
{

}
