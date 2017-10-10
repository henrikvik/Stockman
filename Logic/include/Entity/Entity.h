#ifndef ENTITY_H
#define ENTITY_H

#include "PhysicsObject.h"
#include "StatusManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Logic
{
	class Entity : public PhysicsObject
	{
	public:
		Entity(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

		virtual void clear();
		virtual void update(float deltaTime);

		virtual void affect(int stacks, Effect const &effect, float deltaTime) = 0;
		virtual void upgrade(Upgrade const &upgrade);

		StatusManager& getStatusManager();
		void setStatusManager(StatusManager& statusManager);
	private:
		StatusManager m_statusManager;
	};
}

#endif // !ENTITY_H
