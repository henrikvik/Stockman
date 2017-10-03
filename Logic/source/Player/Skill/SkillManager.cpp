#include "Player/Skill/SkillManager.h"

using namespace Logic;

SkillManager::SkillManager() 
{
	m_currentSkill = nullptr;
	m_canBeUsed = true;
}

SkillManager::~SkillManager()
{
	for (int i = 0; i < m_allSkills.size(); i++)
		delete m_allSkills[i];

	m_currentSkill = nullptr;
	clear();
}

void SkillManager::clear()
{

}

void SkillManager::init(Physics* physics, ProjectileManager* projectileManager, GameTime* gameTime)
{
	m_allSkills =
	{
		{ /*new SkillBulletTime(BULLET_TIME_CD, gameTime)*/ new SkillBulletTime(projectileManager, ProjectileData(0, 1, 1, 100, 0, BULLET_TIME_DURATION, Graphics::ModelID::SPHERE, 1, ProjectileType::ProjectileTypeBulletTime)) },
		{ new SkillGrapplingHook(physics) },
		{ new SkillShieldCharge() }
	};


	switchToSkill(2);
}

void SkillManager::switchToSkill(int index)
{
	m_currentSkill = m_allSkills[index];
}

void SkillManager::useSkill(btVector3 forward, Entity& shooter)
{
	m_canBeUsed = false;

	if (m_currentSkill)
		m_currentSkill->use(forward, shooter);
}

void SkillManager::releaseSkill()
{
	m_canBeUsed = true;

	if (m_currentSkill)
		m_currentSkill->release();
}

void SkillManager::update(float deltaTime)
{
	m_currentSkill->update(deltaTime);
}

void SkillManager::render(Graphics::Renderer& renderer)
{
	m_currentSkill->render(renderer);
}

bool Logic::SkillManager::getCanBeUsed() const
{
	return m_canBeUsed;
}
