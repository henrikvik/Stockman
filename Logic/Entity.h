#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"

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
		virtual void update() = 0;
		void collision();
		virtual void onCollision() = 0;

	private:

	};
}

#endif // !ENTITY_H
