#include "../Projectile/ProjectileManager.h"

using namespace Logic;

ProjectileManager::ProjectileManager(Physics* physPtr)
{
	m_physPtr = physPtr;
}

ProjectileManager::~ProjectileManager() { }

void ProjectileManager::clear()
{
	m_projectiles.clear();
}

void ProjectileManager::addProjectile(ProjectileData& pData, btVector3 forward)
{
	//Projectile* p = newd Projectile(pData);
	//m_projectiles.push_back(p);
}

void ProjectileManager::removeProjectile()
{

}

std::vector<Projectile*>* ProjectileManager::getProjectiles()
{
	return &m_projectiles;
}
