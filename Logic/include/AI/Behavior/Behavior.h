#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Player\Player.h>
#include "AStar.h"

namespace Logic {
	class Behavior {
		public:
			virtual ~Behavior() {}
			virtual void update(Player const &player, float deltaTime) = 0;
	};
}

#endif