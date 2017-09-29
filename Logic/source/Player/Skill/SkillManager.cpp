#include "Player/Skill/SkillManager.h"

using namespace Logic;

SkillManager::SkillManager() 
{
	m_currentSkill = nullptr;
}

SkillManager::~SkillManager()
{
	m_currentSkill = nullptr;
	clear();
}

void SkillManager::clear()
{
	for (int i = 0; i < m_allSkills.size(); i++)
		delete m_allSkills[i];

	m_allSkills.clear();
}

void SkillManager::init(ProjectileManager* projectileManager, GameTime* gameTime)
{
	m_allSkills =
	{
		{ new SkillBulletTime(BULLET_TIME_CD, gameTime) },
		{ new SkillGrapplingHook(50.f, projectileManager, ProjectileData(0, 1, 1, 100, 0, 3000, Graphics::ModelID::SPHERE, 1)) },
		{ new SkillShieldCharge(SHIELD_CHARGE_CD, SHIELD_CHARGE_DURATION, projectileManager, ProjectileData(0, 1, 0, 0, 0, 3000, Graphics::ModelID::CUBE, 1)) }
	};


	switchToSkill(0);
}

void SkillManager::switchToSkill(int index)
{
	m_currentSkill = m_allSkills[index];
}

void SkillManager::useSkill()
{
	if (m_currentSkill)
		m_currentSkill->use();
}

void SkillManager::update(float deltaTime)
{
	m_currentSkill->update(deltaTime);
}

void SkillManager::render(Graphics::Renderer& renderer)
{
	m_currentSkill->render(renderer);
}