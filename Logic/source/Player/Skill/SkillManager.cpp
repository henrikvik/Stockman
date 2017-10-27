#include <DebugDefines.h>
#include "Player/Skill/SkillManager.h"

using namespace Logic;

SkillManager::SkillManager() 
{
    m_current = { nullptr, nullptr };
}

SkillManager::~SkillManager()
{
	for (int i = 0; i < m_allSkills.size(); i++)
		delete m_allSkills[i];

    m_current = { nullptr, nullptr };
	clear();
}

void SkillManager::clear()
{

}

void SkillManager::init(Physics* physics, ProjectileManager* projectileManager, GameTime* gameTime)
{
	m_allSkills =
	{
		{ new SkillBulletTime(projectileManager, ProjectileData(0, 100, 0, 0, 0.f, BULLET_TIME_DURATION, Graphics::ModelID::SPHERE, 1, ProjectileType::ProjectileTypeBulletTimeSensor)) },
		{ new SkillGrapplingHook(physics) },
		{ new SkillShieldCharge() }
	};


    switchToSkill({ SKILL_MOVE, SKILL_UTIL });
}

void SkillManager::switchToSkill(std::pair<int, int> index)
{
    m_current = { m_allSkills[index.first], m_allSkills[index.second] };
}

void SkillManager::usePrimarySkill(btVector3 forward, Entity& shooter)
{
	if (m_current.first)
        m_current.first->use(forward, shooter);
}

void SkillManager::useSecondarySkill(btVector3 forward, Entity& shooter)
{
    if (m_current.second)
        m_current.second->use(forward, shooter);
}

void SkillManager::releasePrimarySkill()
{
	if (m_current.first)
        m_current.first->release();
}

void SkillManager::releaseSecondarySkill()
{
    if (m_current.second)
        m_current.second->release();
}

void SkillManager::update(float deltaTime)
{
    m_current.first->update(deltaTime);
    m_current.second->update(deltaTime);
}

void SkillManager::render(Graphics::Renderer& renderer)
{
    m_current.first->render(renderer);
    m_current.second->render(renderer);
}

Skill * SkillManager::getPrimarySkill() const
{
    return m_current.first;
}

Skill * SkillManager::getSecondarySkill() const
{
    return m_current.second;
}