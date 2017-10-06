#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Player\Player.h>
#include <vector>
#include "SimplePathing.h"

namespace Logic {
	class Enemy;
	class Behavior {
	protected: // ye this is protected, hater
		struct BehaviorNode
		{
			std::vector<BehaviorNode> children;
			//	std::function<bool> condition;
			//	std::function<void> update;
		};
	private:
		BehaviorNode root;
	public:
			virtual ~Behavior() {};
			virtual void update(Enemy &enemy, Player const &player, float deltaTime) = 0;
			virtual void updatePath(Entity const &from, Entity const &to) = 0;
			virtual void debugRendering(Graphics::Renderer &renderer) = 0;
			BehaviorNode& getRoot() { return root; }
	};
}

#endif