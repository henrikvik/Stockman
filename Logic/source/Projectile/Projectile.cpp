#include "../Projectile/Projectile.h"

using namespace Logic;


Projectile::Projectile(btRigidBody* body)
: Entity(body) 
{
	m_damage = 1.f;
	m_speed = 0.f;
	m_gravityModifier = 1.f;
}

Projectile::Projectile(btRigidBody* body, float damage, float speed, float gravityModifer)
: Entity(body)
{
	m_damage = damage;
	m_speed = speed;
	m_gravityModifier = gravityModifer;
}

Projectile::~Projectile() { }

void Projectile::onUpdate(float deltaTime)
{
	
}

float Projectile::getDamage() const { return m_damage; }
float Projectile::getSpeed() const { return m_speed; }
float Projectile::getGravityModifier() const { return m_gravityModifier; }
