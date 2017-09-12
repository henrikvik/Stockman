#ifndef HITBOX_H
#define HITBOX_H

#include "Entity.h"

namespace Logic
{
	class Hitbox : public Entity
	{
	public:
		Hitbox();
		~Hitbox();

		void clear();

		void updateSpecific(float deltatTime);
		void onCollision(Entity& other);

	};
}


#endif // !HITBOX_H
