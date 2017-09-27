#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Player\Player.h>
#include "AStar.h"

namespace Logic {
	class Enemy;
	class Behavior {
		public:
			virtual ~Behavior() {}
			virtual void update(Enemy &enemy, Player const &player, float deltaTime) = 0;
	};
}

#endif