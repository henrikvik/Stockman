#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "StatusManager.h"
#include <Projectile\ProjectileStruct.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <functional>

namespace Logic
{
	class Entity : public Object
	{
	public:
		Entity(btRigidBody* body);
		Entity(btRigidBody* body, std::function<void(ProjectileData* pData, btVector3 forward)> addProjectilePtr);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

		virtual void clear();
		void update(float deltaTime);
		virtual void updateSpecific(float deltaTime) { }
		void collision(Entity& other);
		virtual void affect(int stacks, Effect const &effect,
							float deltaTime);
		virtual void onCollision(Entity& other) { }

		// JUST FOR TESTING, REMOVE
		void consoleWritePosition();

		void spawnProjectile(ProjectileData* pData, btVector3 forward);

		DirectX::SimpleMath::Vector3 getPosition() const;
		DirectX::SimpleMath::Quaternion getRotation() const;
		DirectX::SimpleMath::Vector3 getScale() const;
		DirectX::SimpleMath::Matrix getTransformMatrix() const;

	protected:
		btRigidBody* getRigidbody();

	private:
		StatusManager m_statusManager;
		btRigidBody* m_body;
		btTransform* m_transform;
	//	Hitbox* m_head; for headshot, put it here to remember

		//FuncAddProjectile m_addProjectile;				// Func ptr to ProjectileManager::addProjectile
		std::function<void(ProjectileData* pData, btVector3 forward)> m_addProjectile;
	};
}

#endif // !ENTITY_H
