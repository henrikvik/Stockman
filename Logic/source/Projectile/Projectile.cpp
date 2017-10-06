#include "../Projectile/Projectile.h"
#include "../Player/Player.h"
#include <AI\Enemy.h>

using namespace Logic;


Projectile::Projectile(btRigidBody* body, btVector3 halfextent)
: Entity(body, halfextent) 
{
	m_pData.damage = 1.f;
	m_pData.speed = 0.f;
	m_pData.gravityModifier = 1.f;
	m_pData.ttl = 1000.f;
	m_remove = false;
}

Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, float damage, float speed, float gravityModifer, float ttl)
: Entity(body, halfExtent)
{
	m_pData.damage = damage;
	m_pData.speed = speed;
	m_pData.gravityModifier = gravityModifer;
	m_pData.ttl = ttl;
	m_remove = false;
}

Logic::Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData)
: Entity(body, halfExtent)
{
	m_pData = pData;
	m_remove = false;
	setModelID(pData.meshID);

	switch (pData.type)
	{
		// Do specifics 
	}
}

Projectile::~Projectile() { }

void Projectile::start(btVector3 forward, StatusManager& statusManager)
{
	getRigidbody()->setLinearVelocity(forward * m_pData.speed);
	setStatusManager(statusManager);

	for (StatusManager::UPGRADE_ID id : statusManager.getActiveUpgrades())
		upgrade(statusManager.getUpgrade(id));
}

void Projectile::updateSpecific(float deltaTime)
{
	m_pData.ttl -= deltaTime;
}

void Projectile::onCollision(Entity & other, const btRigidBody* collidedWithYour)
{
	// TEMP
	Player* p = dynamic_cast<Player*>(&other);
	Projectile* proj = dynamic_cast<Projectile*> (&other);

	if (proj)
	{
		
	}
	else if (p && m_pData.enemyBullet)
	{
		p->takeDamage(m_pData.damage);
		m_remove = true;

		for (StatusManager::UPGRADE_ID upgrade : this->getStatusManager().getActiveUpgrades())
			if (this->getStatusManager().getUpgrade(upgrade).getTranferEffects() & Upgrade::UPGRADE_IS_BOUNCING)
				m_remove = false;
	}
	else if (Enemy *enemy = dynamic_cast<Enemy*>(&other))
	{
		if (!m_pData.enemyBullet)
			enemy->damage(m_pData.damage);
	}
}

void Projectile::upgrade(Upgrade const &upgrade)
{
	long long flags = upgrade.getTranferEffects();

	if (flags & Upgrade::UPGRADE_INCREASE_DMG)
	{
		m_pData.damage = upgrade.getFlatUpgrades().increaseDmg;
	}
	if (flags & Upgrade::UPGRADE_IS_BOUNCING)
	{
		this->getRigidbody()->setRestitution(1.f);
	}
}

ProjectileType Projectile::getType() const { return m_type; }
// REPLACE THIS WITH GET STRUCT INSTEAD
ProjectileData& Projectile::getProjectileData() { return m_pData; }

void Logic::Projectile::toRemove()
{
	m_remove = true;
}

bool Logic::Projectile::shouldRemove() const
{
	return m_remove;
}
