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

void SkillManager::init()
{
	initializeSkills();
	switchToSkill(0);
}

void SkillManager::initializeSkills()
{
	m_allSkills =
	{
		{ new SkillBulletTime() },
		{ new SkillGrapplingHook() },
		{ new SkillShieldCharge() }
	};
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