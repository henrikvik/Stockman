#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "../Physics/Physics.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Keyboard.h"

namespace Logic
{
	class Entity : public Object
	{
	public:
		Entity();
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		~Entity();

		void createRigidBody(Physics* physics);

		virtual void clear() = 0;
		void update(float deltaTime);
		virtual void updateSpecific(float deltatTime) = 0;
		void collision(Entity& other);
		virtual void onCollision(Entity& other) = 0;

	private:
		btRigidBody* m_rigidBody;
	};
}

#endif // !ENTITY_H
