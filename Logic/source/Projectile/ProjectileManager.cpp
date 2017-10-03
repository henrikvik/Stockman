#include "../Projectile/ProjectileManager.h"

using namespace Logic;

ProjectileManager::ProjectileManager(Physics* physPtr)
{
	m_physPtr = physPtr;
}

ProjectileManager::~ProjectileManager() { }

void ProjectileManager::clear()
{
	for (Projectile* p : m_projectiles)
		delete p;
	m_projectiles.clear();
}

void ProjectileManager::addProjectile(ProjectileData& pData, btVector3 position, btVector3 forward, Entity& shooter)
{
	// Create body
	btRigidBody* body = m_physPtr->createBody(Cube(position + (forward * 2), btVector3(), { pData.scale, pData.scale, pData.scale }), pData.mass, false);
	// Set gravity modifier
	body->setGravity(pData.gravityModifier * m_physPtr->getGravity());
	// Create projectile
	Projectile* p = newd Projectile(body, { pData.scale, pData.scale, pData.scale }, pData);
	// Start
	p->start(forward, shooter.getStatusManager());

	// Add to projectile list
	m_projectiles.push_back(p);
	//printf("projs: %d\n", m_projectiles.size());
}

void ProjectileManager::removeProjectile(Projectile* p)
{
	m_physPtr->removeRigidBody(p->getRigidbody());
	p->destroyBody();
	delete p;
	
	int found = -1;
	size_t nrProjectiles = m_projectiles.size();
	for (size_t i = nrProjectiles; i-- && found == -1; )
	{
		if (m_projectiles[nrProjectiles - (i + 1)] == p)
			found = i + 1;
	}

	m_projectiles.erase(m_projectiles.begin() + found);
}

void ProjectileManager::removeProjectile(Projectile* p, int index)
{
	m_physPtr->removeRigidBody(p->getRigidbody());
	p->destroyBody();
	delete p;
	m_projectiles.erase(m_projectiles.begin() + index);
}

void Logic::ProjectileManager::update(float deltaTime)
{
	for (size_t i = 0; i < m_projectiles.size(); i++)
	{
		Projectile* p = m_projectiles[i];
		p->update(deltaTime);
		if (p->shouldRemove() || p->getProjectileData().ttl < 0.f)		// Check remove flag and ttl
		{
			removeProjectile(p, i);
			i--;
		}
	}
}

void Logic::ProjectileManager::render(Graphics::Renderer& renderer)
{
	for (Projectile* p : m_projectiles)
		p->render(renderer);
}

std::vector<Projectile*>* ProjectileManager::getProjectiles()
{
	return &m_projectiles;
}
