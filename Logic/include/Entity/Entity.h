#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "StatusManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Logic
{
	class Entity : public Object
	{
	public:
		Entity(btRigidBody* body);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

		virtual void clear();
		void update(float deltaTime);
		void updateGraphics();
		virtual void updateSpecific(float deltaTime) { }
		void collision(Entity& other);
		virtual void affect(int stacks, Effect const &effect,
							float deltaTime);
		virtual void onCollision(Entity& other) { }

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
	};
}

#endif // !ENTITY_H
