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
	this->setModelID(Graphics::ModelID::SPHERE);
}

Projectile::~Projectile() { }

void Logic::Projectile::start(btVector3 forward)
{
	getRigidbody()->setLinearVelocity(forward * m_speed);
}

void Projectile::updateSpecific(float deltaTime)
{
	m_ttl -= deltaTime;
}

void Logic::Projectile::onCollision(Entity & other)
{
	// TEMP
	Player* p = dynamic_cast<Player*>(&other);
	if (!p)	m_remove = true;;
	
}

float Projectile::getDamage() const { return m_damage; }
float Projectile::getSpeed() const { return m_speed; }
float Projectile::getGravityModifier() const { return m_gravityModifier; }

float Logic::Projectile::getTTL() const { return m_ttl; }

void Logic::Projectile::toRemove()
{
	m_remove = true;
}

bool Logic::Projectile::shouldRemove() const
{
	return m_remove;
}
