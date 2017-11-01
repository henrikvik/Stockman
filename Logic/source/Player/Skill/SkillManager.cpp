#include <DebugDefines.h>
#include "Player/Skill/SkillManager.h"
#include <Player\Skill\SkillBulletTime.h>
#include <Player\Skill\SkillGrapplingHook.h>
#include <Player\Skill\SkillShieldCharge.h>
#include <Projectile\ProjectileManager.h>
#include <Entity\Entity.h>

using namespace Logic;

SkillManager::SkillManager() 
{
    for (int i = 0; i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i] = nullptr;
}

SkillManager::~SkillManager()
{
    // Deleting all skills
	for (int i = 0; i < m_allSkills.size(); i++)
		delete m_allSkills[i];

    // Resetting pointers
    for (int i = 0; i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i] = nullptr;
	
    clear();
}

void SkillManager::clear()
{

}

void SkillManager::init(Physics* physics, ProjectileManager* projectileManager)
{
	m_allSkills =
	{
		{ new SkillBulletTime(projectileManager, ProjectileData(0, 100, 0, 0, 0.f, BULLET_TIME_DURATION, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeBulletTimeSensor)) },
		{ new SkillGrapplingHook(physics) },
		{ new SkillShieldCharge() }
	};

    switchToSkill({ 0, 1, 2 });
}

void SkillManager::switchToSkill(std::vector<int> skillsToUse)
{
    if (skillsToUse.size() > THRESHOLD::MAX_SKILLS)
        printf("The number of skills is breaking the threshold. The player don't have space for more.");

    for (size_t i = 0;  i < skillsToUse.size() ||
                        i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i] = m_allSkills[skillsToUse[i]];
}

void SkillManager::use(int index, btVector3 forward, Entity& shooter)
{
    if (isLegit(index))
    {
        if (m_current[index])
        {
             m_current[index]->use(forward, shooter);
        }
    }
}

void SkillManager::release(int index)
{
    if (isLegit(index))
    {
        if (m_current[index])
        {
            m_current[index]->release();
        }
    }
}

void SkillManager::update(float deltaTime)
{
    for (size_t i = 0; i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i]->update(deltaTime);
}

void SkillManager::render()
{
    for (size_t i = 0; i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i]->render();
}

Skill* SkillManager::getSkill(int index) const
{
    if (!isLegit(index))
        return nullptr;
    return m_current[index];
}

bool SkillManager::isLegit(int index) const
{
    return (!(index >= THRESHOLD::MAX_SKILLS && index < NULL));
}