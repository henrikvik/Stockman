#include "../Projectile/ProjectileManager.h"

using namespace Logic;

ProjectileManager::ProjectileManager()
{

}

ProjectileManager::~ProjectileManager() { }

void ProjectileManager::clear()
{
	m_projectiles.clear();
}

void ProjectileManager::addProjectile(ProjectileData& pData)
{
//	Projectile* p = new Projectile(pData);
//	m_projectiles.push_back(p);
}

void ProjectileManager::removeProjectile()
{

}

std::vector<Projectile*>* ProjectileManager::getProjectiles()
{
	return &m_projectiles;
}
