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
		Entity(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

		void addWeakpoint(btRigidBody* body, btVector3 offset);
		void destroyBody();
		virtual void clear();
		virtual void update(float deltaTime);
		void updateGraphics();
		virtual void updateSpecific(float deltaTime) { }
		void collision(Entity& other, btVector3 contactPoint, const btRigidBody* collidedWithYour);
		virtual void affect(int stacks, Effect const &effect, float deltaTime);	
		virtual void upgrade(Upgrade const &upgrade);
		virtual void onCollision(Entity& other, btVector3 contactPoint, float dmgMultiplier) { }

		void setHalfExtent(btVector3 halfExtent);
		btVector3 getHalfExtent() const;
		DirectX::SimpleMath::Vector3 getPosition() const;
		btVector3 getPositionBT() const;
		DirectX::SimpleMath::Quaternion getRotation() const;
		DirectX::SimpleMath::Vector3 getScale() const;
		DirectX::SimpleMath::Matrix getTransformMatrix() const;

		btRigidBody* getRigidbody();
		StatusManager& getStatusManager();
		void setStatusManager(StatusManager& statusManager);
	private:

		StatusManager m_statusManager;
		btVector3 m_halfextent;
		btRigidBody* m_body;
		btTransform* m_transform;

		btRigidBody* m_bodyWeakPoint;	//< The "head" for headshots multiplier
		btVector3 m_weakPointOffset;	//< The constant offset from the body
	};
}

#endif // !ENTITY_H
