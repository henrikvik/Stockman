#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Player\Player.h>
#include <vector>
#include "SimplePathing.h"

namespace Logic {
	class Enemy;
	class Behavior {
	protected: // This is protected at the moment, it will maybe be changed later, better solutions is welcome'd
		// this is not complete just a thought
		struct RunIn
		{
			Enemy *enemy;
			std::vector<Enemy*> closeEnemies;
			const Player *target;
			Behavior *behavior;
			float deltaTime;
		};

		typedef bool(*run)(RunIn&);

		enum NodeType
		{
			PRIORITY,
			RANDOM,
			CONDITION,
			ACTION,
			SEQUENCE
		};
		struct BehaviorNode
		{
			NodeType type;
			int value; // value is dependent on node type
			std::vector<BehaviorNode> children;

			std::function<bool(RunIn&)> run; // return is fail / success
			bool operator> (BehaviorNode const &other)
			{
				return value > other.value;
			}
		};
	private:
		BehaviorNode m_root;
	public:
		virtual ~Behavior();

		virtual void update(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
			Player const &player, float deltaTime) = 0;
		virtual void updatePath(Entity const &from, Entity const &to) = 0;
		virtual void debugRendering(Graphics::Renderer &renderer) = 0;

		virtual void walkPath(SimplePathing pathing, RunIn &runIn);

		void runTree(RunIn &in);
		bool runNode(RunIn &in, BehaviorNode &node);
		
		void setRoot(NodeType type, int value, run func);
		BehaviorNode* addNode(BehaviorNode *parent, NodeType type, int value, run func);

		BehaviorNode* getRoot();
	};
}

#endif