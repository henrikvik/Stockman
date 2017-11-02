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
    m_nrOfSkills = 0;

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
	
    m_nrOfSkills = 0;
}

void SkillManager::init(Physics* physics, ProjectileManager* projectileManager)
{
	m_allSkills =
	{
		{ newd SkillGrapplingHook(physics) },
		{ newd SkillShieldCharge() },
        { newd SkillBulletTime(projectileManager, ProjectileData(0, 100, 1.f, 0, 0.f, BULLET_TIME_DURATION, Resources::Models::UnitCube, 1, ProjectileType::ProjectileTypeBulletTimeSensor, true)) }
    };

    switchToSkill({ SKILL_SHIELD_CHARGE, SKILL_GRAPPLING_HOOK, SKILL_BULLET_TIME });
}

// Switches out currently active skills to a new list of skill, by using the parameter's skill indexes
void SkillManager::switchToSkill(std::vector<SKILL> skillsToUse)
{
    if (skillsToUse.size() > THRESHOLD::MAX_SKILLS)
        printf("The number of skills is breaking the threshold. The player don't have space for more.");

    // Reset skills
    for (size_t i = 0; i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i] = nullptr;

    // Initialize new skills
    for (size_t i = 0;  i < skillsToUse.size() &&
                        i < THRESHOLD::MAX_SKILLS; i++)
        m_current[i] = m_allSkills[skillsToUse[i]];

    // Saving the number of skills currently active
    m_nrOfSkills = (short)skillsToUse.size();
}

// Trying to use a skill of a specific index
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

// Trying to release a skill of a specific index
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

// Updating EVERY skill
void SkillManager::update(float deltaTime)
{
    for (size_t i = 0; i < m_nrOfSkills; i++)
        m_current[i]->update(deltaTime);
}

// Drawing EVERY skill
void SkillManager::render()
{
    for (size_t i = 0; i < m_nrOfSkills; i++)
        m_current[i]->render();
}

// Returns a pointer to a specific skill in an indexed position (0 - primary, 1 - secondary, 2 - tr)
Skill* SkillManager::getSkill(int index) const
{
    if (!isLegit(index))
        return nullptr;
    return m_current[index];
}

// If the index parameter is inbetween the valid variables
bool SkillManager::isLegit(int index) const
{
    return (!(index >= m_nrOfSkills && index < NULL));
}