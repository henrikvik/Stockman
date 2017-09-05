#ifndef PLAYER_H
#define PLAYER_H

#include "Entity\Entity.h"

namespace Logic
{
	class Player : public Entity
	{
	public:
		Player();
		~Player();

		void clear();
		void update();
		void onCollision(const Entity& other);

	};

}

#endif // !PLAYER_H
