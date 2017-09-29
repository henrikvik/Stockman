#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Entity/Entity.h>
#include <Projectile\ProjectileStruct.h>

namespace Logic
{
	class Projectile : public Entity
	{
	public:

		Projectile(btRigidBody* body, btVector3 halfExtent);
		Projectile(btRigidBody* body, btVector3 halfExtent, float damage, float speed, float gravityModifer, float ttl);
		Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData);
		Projectile(const Projectile& other) = delete;
		Projectile* operator=(const Projectile& other) = delete;
		~Projectile();

		void start(btVector3 forward, StatusManager& statusManager);
		void updateSpecific(float deltaTime);
		void onCollision(Entity& other);
		void upgrade(Upgrade const &upgrade);

		float getDamage() const;
		float getSpeed() const;
		float getGravityModifier() const;
		float getTTL() const;
		void setDamage(float damage);
		void setSpeed(float speed);
		void setGravityModifier(float gravityModifier);

		void toRemove();
		bool shouldRemove() const;

	private:
		float m_damage;
		float m_speed;
		float m_gravityModifier;
		float m_ttl;
		bool m_remove;
	};
}

#endif // !PROJECTILE_H
