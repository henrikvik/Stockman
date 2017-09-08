#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Logic
{
	class Entity : public Object
	{
	public:
		Entity();
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		~Entity();

		virtual void clear() = 0;
		void update(float deltaTime);
		virtual void updateSpecific(float deltatTime) = 0;
		void collision(const Entity& other);
		virtual void onCollision(const Entity& other) = 0;

	private:

	};
}

#endif // !ENTITY_H
