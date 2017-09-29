#include "Player/Skill/SkillBulletTime.h"

using namespace Logic;

SkillBulletTime::SkillBulletTime(float cooldown, GameTime * gameTime)
: Skill(cooldown)
{
	m_gameTime = gameTime;
}

void SkillBulletTime::onUse(btVector3 forward, Entity& shooter)
{
	printf("Bullet Time used.\n");
	m_gameTime->slowMotion(0.40f, 5000.f);
}

void SkillBulletTime::onUpdate(float deltaTime)
{

}

void SkillBulletTime::render(Graphics::Renderer& renderer)
{

}
