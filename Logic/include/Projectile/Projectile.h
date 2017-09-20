#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Entity/Entity.h>
#include <Projectile\ProjectileStruct.h>

namespace Logic
{
	class Projectile : public Entity
	{
	public:
		Projectile(btRigidBody* body);
		Projectile(btRigidBody* body, float damage, float speed, float gravityModifer, float ttl);
		Projectile(btRigidBody* body, ProjectileData pData);
		Projectile(const Projectile& other) = delete;
		Projectile* operator=(const Projectile& other) = delete;
		~Projectile();

		void start(btVector3 forward);
		void updateSpecific(float deltaTime);

		float getDamage() const;
		float getSpeed() const;
		float getGravityModifier() const;
		float getTTL() const;

	private:
		float m_damage;
		float m_speed;
		float m_gravityModifier;
		float m_ttl;
	};
}

#endif // !PROJECTILE_H
