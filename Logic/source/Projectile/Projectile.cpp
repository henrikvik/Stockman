#include "../Projectile/Projectile.h"
#include "../Player/Player.h"

using namespace Logic;


Projectile::Projectile(btRigidBody* body, btVector3 halfextent)
: Entity(body, halfextent) 
{
	m_pData.damage = 1.f;
	m_pData.speed = 0.f;
	m_pData.gravityModifier = 1.f;
	m_pData.ttl = 1000.f;
	m_remove = false;
	m_speedMod = 1.f;
}

Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, float damage, float speed, float gravityModifer, float ttl)
: Entity(body, halfExtent)
{
	m_pData.damage = damage;
	m_pData.speed = speed;
	m_pData.gravityModifier = gravityModifer;
	m_pData.ttl = ttl;
	m_remove = false;
	m_speedMod = 1.f;
}

Logic::Projectile::Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData)
: Entity(body, halfExtent)
{
	m_pData = pData;
	m_remove = false;
	setModelID(pData.meshID);
	m_speedMod = 1.f;

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
	btVector3 vel = getRigidbody()->getLinearVelocity().normalized();
	getRigidbody()->setLinearVelocity(vel * m_pData.speed * m_speedMod);
	getRigidbody()->setGravity({ 0, PHYSICS_GRAVITY * m_pData.gravityModifier * m_speedMod, 0.f });

	if (m_pData.type == ProjectileTypeBulletTimeSensor)
		m_pData.ttl -= deltaTime;
	else
		m_pData.ttl -= deltaTime * m_speedMod;
	m_speedMod = 1.f;
}

void Projectile::onCollision(Entity & other, btVector3 contactPoint, const btRigidBody* collidedWithYour)
{
	// TEMP
	Player* p = dynamic_cast<Player*>(&other);
	Projectile* proj = dynamic_cast<Projectile*> (&other);

	if (proj)
	{
		if(proj->getProjectileData().type)
			setStatusManager(proj->getStatusManager());
	}
	else if ((p && m_pData.enemyBullet) || (!p && !m_pData.enemyBullet))
	{
		m_remove = true;

		for (StatusManager::UPGRADE_ID upgrade : this->getStatusManager().getActiveUpgrades())
			if (this->getStatusManager().getUpgrade(upgrade).getTranferEffects() & Upgrade::UPGRADE_IS_BOUNCING)
				m_remove = false;
	}

	if (m_pData.type == ProjectileTypeBulletTimeSensor)
		m_remove = false;
}

void Projectile::affect(int stacks, Effect const &effect, float dt)
{
	int flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_MODIFY_MOVEMENTSPEED)
	{
		m_speedMod *= std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
		//if(m_pData.type != ProjectileTypeBulletTimeSensor)
			//printf("%d\n", stacks);
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

ProjectileData& Projectile::getProjectileData() { return m_pData; }

void Logic::Projectile::toRemove()
{
	m_remove = true;
}

bool Logic::Projectile::shouldRemove() const
{
	return m_remove;
}
