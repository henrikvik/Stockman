#include "Player/Skill/SkillBulletTime.h"

using namespace Logic;

SkillBulletTime::SkillBulletTime(GameTime * gameTime)
{
	m_gameTime = gameTime;
}

void SkillBulletTime::onUse()
{
	m_gameTime->slowMotion(0.40f, 5000.f);
}

void SkillBulletTime::onUpdate(float deltaTime)
{

}

void SkillBulletTime::render(Graphics::Renderer& renderer)
{

}
