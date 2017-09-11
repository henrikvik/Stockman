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

		bool init(Physics* physics, BodyDesc bodyDesc);
	};
}


#endif // !HITBOX_H
