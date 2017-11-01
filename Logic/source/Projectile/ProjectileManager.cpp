#include "../Projectile/ProjectileManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define PROJECTILE_DEFAULT_COUNT 50
#define PROJECTILE_DEFAULT_POS { 0.f, -100.f, 0.f }

using namespace Logic;

ProjectileManager::ProjectileManager(Physics* physPtr)
{
	m_physPtr = physPtr;
    init();
}

ProjectileManager::~ProjectileManager() 
{

}

void Logic::ProjectileManager::init()
{
    for (size_t i = PROJECTILE_DEFAULT_COUNT; i--;)
    {
        // Create body
        btRigidBody* body = m_physPtr->createBody(Sphere(PROJECTILE_DEFAULT_POS, { 0.f, 0.f, 0.f }, 1.f), 1.f, false, 0, 0);

        // benching body
        m_physPtr->removeRigidBody(body);

        body->setGravity({ 0.f, 0.f, 0.f });

        Projectile* p = newd Projectile(body, { 0.f, 0.f, 0.f }, ProjectileData());

        m_projectilesIdle.push_back(p);
    }
}

void ProjectileManager::clear()
{
    for (Projectile* p : m_projectilesActive)
        delete p;
	m_projectilesActive.clear();

    for (Projectile* p : m_projectilesIdle)
    {
        p->destroyBody();
        delete p;
    }
    m_projectilesIdle.clear();
}

Projectile* ProjectileManager::addProjectile(ProjectileData& pData, btVector3 position, btVector3 forward, Entity& shooter)
{
    btRigidBody* body;
    Projectile* p;

    // creating new projectile if idle stack is empty
    if (m_projectilesIdle.size() == 0)  // it shouldn't get to this, because it will lag in debug mode
    {
        body = m_physPtr->createBody(Sphere({ position + (forward * 2) }, { 0.f, 0.f, 0.f }, pData.scale), pData.mass, pData.isSensor, 0, 0);
        p = newd Projectile(body, { pData.scale, pData.scale, pData.scale }, pData);
    }
    // using projectile from idle stack
    else
    {
        // fetching idle projectile
        p = m_projectilesIdle.back();
        m_projectilesIdle.pop_back();
        p->setProjectileData(pData);
        p->setModelID(pData.meshID);
        p->setHalfExtent({ pData.scale, pData.scale, pData.scale });
        // fetching body
        body = p->getRigidBody();
        m_physPtr->addRigidBody(body);
        
        // isSensor
        if (pData.isSensor)
            body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        // mass
        body->setMassProps(pData.mass, { 0.f, 0.f, 0.f });
        // scale
        body->getCollisionShape()->setLocalScaling({ pData.scale, pData.scale, pData.scale });
        m_physPtr->updateSingleAabb(body);
        // position
        body->getWorldTransform().setOrigin({ position + (forward * 2) });
    }

    // collision group
    body->getBroadphaseProxy()->m_collisionFilterGroup =
        (pData.enemyBullet) ? (Physics::COL_EN_PROJ) : (Physics::COL_PL_PROJ);
    // collision mask
    body->getBroadphaseProxy()->m_collisionFilterMask = 
        (pData.enemyBullet) ? (Physics::COL_EVERYTHING &~(Physics::COL_ENEMY)) : (Physics::COL_EVERYTHING &~(Physics::COL_PLAYER));

	// Taking the forward vector and getitng the pitch and yaw from it
	float pitch = asin(-forward.getY());
	float yaw = atan2(forward.getX(), forward.getZ());
	btQuaternion q(yaw, pitch, 0.f);
	body->getWorldTransform().setRotation(btQuaternion(yaw, pitch - float(180 * M_PI / 180), 0.f));

	// Set gravity modifier
	body->setGravity(pData.gravityModifier * m_physPtr->getGravity());

	// Start
	p->start(forward, shooter.getStatusManager());

	// Add to active-list
	m_projectilesActive.push_back(p);

	return p;
}

void ProjectileManager::removeProjectile(Projectile* p)
{
	m_physPtr->removeRigidBody(p->getRigidBody());
	p->destroyBody();
	delete p;
	
	int found = -1;
	size_t nrProjectiles = m_projectilesActive.size();
	for (size_t i = nrProjectiles; i-- && found == -1; )
	{
		if (m_projectilesActive[nrProjectiles - (i + 1)] == p)
			found = (int)i + 1;
	}

	m_projectilesActive.erase(m_projectilesActive.begin() + found);
}

void ProjectileManager::removeProjectile(Projectile* p, int index)
{
    if (m_projectilesIdle.size() >= PROJECTILE_DEFAULT_COUNT)
    {
        m_physPtr->removeRigidBody(p->getRigidBody());
        p->destroyBody();
        delete p;
    }
    else
    {
        btRigidBody* body = p->getRigidBody();

        // reset stuff
        body->setLinearVelocity({ 0.f, 0.f, 0.f });
        body->setGravity({ 0.f, 0.f, 0.f });
        body->getWorldTransform().setOrigin(PROJECTILE_DEFAULT_POS);
        p->setWorldTranslation(p->getTransformMatrix());

        // reset collision flags
        if (p->getProjectileData().isSensor)
            body->setCollisionFlags(body->getCollisionFlags() &~ btCollisionObject::CF_NO_CONTACT_RESPONSE);
        body->getBroadphaseProxy()->m_collisionFilterGroup = 0;
        body->getBroadphaseProxy()->m_collisionFilterMask = 0;
        // benching body
        m_physPtr->removeRigidBody(body);

        // dont remove again duh
        p->toRemove(false);

        // remove all callbacks from the projectile
        p->getCallbacks().clear();

        // add to idle stack
        m_projectilesIdle.push_back(p);
    }
	m_projectilesActive.erase(m_projectilesActive.begin() + index);
}

void Logic::ProjectileManager::update(float deltaTime)
{
	for (size_t i = 0; i < m_projectilesActive.size(); i++)
	{
		Projectile* p = m_projectilesActive[i];
		p->updateSpecific(deltaTime);
		if (p->shouldRemove() || p->getProjectileData().ttl < 0.f)		// Check remove flag and ttl
		{
			removeProjectile(p, (int)i);
			i--;
		}
	}
}

void Logic::ProjectileManager::render(Graphics::Renderer& renderer)
{
	for (Projectile* p : m_projectilesActive)
		p->render(renderer);
}

std::vector<Projectile*>& ProjectileManager::getProjectiles()
{
	return m_projectilesActive;
}
