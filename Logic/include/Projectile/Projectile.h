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
		void onCollision(Entity& other, btVector3 contactPoint, float dmgMultiplier);
		void upgrade(Upgrade const &upgrade);

		ProjectileType getType() const;
		ProjectileData& getProjectileData();

		void toRemove();
		bool shouldRemove() const;

	private:
		ProjectileType m_type;
		ProjectileData m_pData;
		bool m_remove;
	};
}

#endif // !PROJECTILE_H
