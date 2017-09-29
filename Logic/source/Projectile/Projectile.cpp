#include "../Projectile/Projectile.h"
#include "../Player/Player.h"

using namespace Logic;


Projectile::Projectile(btRigidBody* body, btVector3 halfextent)
: Entity(body, halfextent) 
{
	m_damage = 1.f;
	m_speed = 0.f;
	m_gravityModifier = 1.f;
	m_ttl = 1000.f;
	m_remove = false;
}

Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, float damage, float speed, float gravityModifer, float ttl)
: Entity(body, halfExtent)
{
	m_damage = damage;
	m_speed = speed;
	m_gravityModifier = gravityModifer;
	m_ttl = ttl;
	m_remove = false;
}

Logic::Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData)
: Entity(body, halfExtent)
{
	m_damage = pData.damage;
	m_speed = pData.speed;
	m_gravityModifier = pData.gravityModifier;
	m_ttl = pData.ttl;
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
	getRigidbody()->setLinearVelocity(forward * m_speed);
	setStatusManager(statusManager);

	for (StatusManager::UPGRADE_ID id : statusManager.getActiveUpgrades())
		upgrade(statusManager.getUpgrade(id));
}

void Projectile::updateSpecific(float deltaTime)
{
	m_ttl -= deltaTime;
}

void Projectile::onCollision(Entity & other)
{
	// TEMP
	Player* p = dynamic_cast<Player*>(&other);
	if (!p)
	{
		m_remove = true;

		for (StatusManager::UPGRADE_ID upgrade : this->getStatusManager().getActiveUpgrades())
			if (this->getStatusManager().getUpgrade(upgrade).getTranferEffects() & Upgrade::UPGRADE_IS_BOUNCING)
				m_remove = false;
	}
}

void Projectile::upgrade(Upgrade const &upgrade)
{
	long long flags = upgrade.getTranferEffects();

	if (flags & Upgrade::UPGRADE_INCREASE_DMG)
	{
		this->setDamage(upgrade.getFlatUpgrades().increaseDmg);
	}
	if (flags & Upgrade::UPGRADE_IS_BOUNCING)
	{
		this->getRigidbody()->setRestitution(1.f);
	}
}

float Projectile::getDamage() const { return m_damage; }
float Projectile::getSpeed() const { return m_speed; }
float Projectile::getGravityModifier() const { return m_gravityModifier; }
void Projectile::setDamage(float damage) { m_damage = damage; }
void Projectile::setSpeed(float speed) { m_speed = speed; }
void Projectile::setGravityModifier(float gravityModifier) { m_gravityModifier = gravityModifier; }

float Logic::Projectile::getTTL() const { return m_ttl; }

void Logic::Projectile::toRemove()
{
	m_remove = true;
}

bool Logic::Projectile::shouldRemove() const
{
	return m_remove;
}
