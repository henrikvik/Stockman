#include "../Projectile/ProjectileManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <Physics\Physics.h>
#include <Projectile\Projectile.h>

#define PROJECTILE_DEFAULT_COUNT 400 
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

void ProjectileManager::init()
{
    for (size_t i = PROJECTILE_DEFAULT_COUNT; i--;)
    {
        // Create body
        btRigidBody* body = m_physPtr->createBody(Cube(PROJECTILE_DEFAULT_POS, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }), 1.f, false, 0, 0);

        // benching body
        m_physPtr->removeRigidBody(body);

        body->setGravity({ 0.f, 0.f, 0.f });

        Projectile* p = newd Projectile(body, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, ProjectileData());

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

Projectile* ProjectileManager::addProjectile(ProjectileData& pData, btVector3 position, btVector3 forward, Entity& shooter, btVector3 modelOffset)
{
    btRigidBody* body;
    Projectile* p;

    // creating new projectile if idle stack is empty
    if (m_projectilesIdle.size() == 0)  // it shouldn't get to this, because it will lag in debug mode
    {
        return nullptr;
        body = m_physPtr->createBody(Cube({ position + forward }, { 0.f, 0.f, 0.f }, pData.hitboxScale), pData.mass, pData.isSensor, 0, 0);
        p = newd Projectile(body, pData.modelScale, modelOffset, pData);
    }
    // using projectile from idle stack
    else
    {
        // fetching idle projectile
        p = m_projectilesIdle.back();
        m_projectilesIdle.pop_back();
        p->setProjectileData(pData);
        p->setModelID(pData.meshID);
        p->setHalfExtent(pData.modelScale);
        p->setModelOffset(modelOffset);
        p->setUnrotatedMO(modelOffset);
        // fetching body
        body = p->getRigidBody();
        // adding the body to physics world
        m_physPtr->addRigidBody(body);
        p->getStatusManager().clear();
        
        // isSensor
        if (pData.isSensor)
            body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        // mass
        body->setMassProps(pData.mass, { 0.f, 0.f, 0.f });
        // scale
        body->getCollisionShape()->setLocalScaling(pData.hitboxScale);
        m_physPtr->updateSingleAabb(body);
        // position
        body->getWorldTransform().setOrigin({ position + forward });
    }

    // collision group
    body->getBroadphaseProxy()->m_collisionFilterGroup =
        (pData.enemyBullet) ? (Physics::COL_EN_PROJ) : (Physics::COL_PL_PROJ);
    // collision mask
    body->getBroadphaseProxy()->m_collisionFilterMask = 
        (pData.enemyBullet) ? (Physics::COL_EVERYTHING &~ (Physics::COL_ENEMY | Physics::COL_EN_PROJ)) : (Physics::COL_EVERYTHING &~(Physics::COL_PLAYER | Physics::COL_PL_PROJ));

	// Taking the forward vector and getting the pitch and yaw from it
	float pitch = asin(-forward.getY()) - M_PI;
	float yaw = atan2(forward.getX(), forward.getZ());
    //float roll = RandomGenerator::singleton().getRandomFloat(0.f, 2.f * M_PI); // Random roll rotation
	body->getWorldTransform().setRotation(btQuaternion(yaw, pitch - M_PI, 0));

	// Set gravity modifier
	body->setGravity(pData.gravityModifier * m_physPtr->getGravity());

	// Start
	p->start(forward, shooter.getStatusManager());

	// Add to active-list
	m_projectilesActive.push_back(p);
	return p;
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
        // remove all callbacks from the projectile
        p->clearCallbacks(true);

        btRigidBody* body = p->getRigidBody();

        // reset stuff
        body->setLinearVelocity({ 0.f, 0.f, 0.f });
        body->setGravity({ 0.f, 0.f, 0.f });
        body->getWorldTransform().setOrigin(PROJECTILE_DEFAULT_POS);
        //p->setWorldTransform(p->getTransformMatrix());

        // reset collision flags
        if (p->getProjectileData().isSensor)
            body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
        body->getBroadphaseProxy()->m_collisionFilterGroup = 0;
        body->getBroadphaseProxy()->m_collisionFilterMask = 0;
        // benching body
        m_physPtr->removeRigidBody(body);

        // dont remove again duh
        p->setDead(false);

        // clear uppgrades and effects on projectile
        p->getStatusManager().clear();

        p->getProjectileData().hasEffect = false;

        // add to idle stack
        m_projectilesIdle.push_back(p);
    }
    std::swap(m_projectilesActive[index], m_projectilesActive[m_projectilesActive.size() - 1]);
    m_projectilesActive.pop_back();
}

void ProjectileManager::update(float deltaTime)
{
	for (size_t i = 0; i < m_projectilesActive.size(); i++)
	{
		Projectile* p = m_projectilesActive[i];
		p->updateSpecific(deltaTime);
		if (p->getDead())		// Check remove flag and ttl
		{
			removeProjectile(p, (int)i);
			i--;
		}
	}
}

void Logic::ProjectileManager::render()
{
	for (Projectile* p : m_projectilesActive)
		p->render();
}

void ProjectileManager::removeAllProjectiles()
{
    while (!m_projectilesActive.empty())
        removeProjectile(m_projectilesActive[0], 0);
}

void Logic::ProjectileManager::removeEnemyProjCallbacks()
{
    for (Projectile* p : m_projectilesActive)
    {
        if (p->getProjectileData().enemyBullet)
            p->clearCallbacks(true);
    }
}

std::vector<Projectile*>& ProjectileManager::getProjectiles()
{
	return m_projectilesActive;
}
