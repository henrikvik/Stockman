#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Player\Player.h>
#include <vector>
#include <AI\Pathing\Pathing.h>

// this class is pretty bloated but it is neccessary, may make it better in future

namespace Logic {
	class Enemy;
	class Behavior {
	protected: // This is protected at the moment, it will maybe be changed later, better solutions is welcome'd
		// this is not complete just a thought

		// input for calculations, can probably be better, a TODO
		struct RunIn
		{
			Enemy *enemy;
			std::vector<Enemy*> closeEnemies;
			Player *target;
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
        static const float STEERING_BASE, STEERING_MOD;

		BehaviorNode m_root;
		Pathing m_pathing;
		bool m_changedGoalNode;
        float m_steeringSpeed;

		bool runNode(RunIn &in, BehaviorNode &node);
	public:
		enum PathingType
		{
			NO_PATHING,
			CHASING,
			FLANKING
		};

		Behavior(PathingType type);
		Behavior(Behavior const &behavior) = delete;
		Behavior* operator=(Behavior const &other) = delete;
		virtual ~Behavior();

		void update(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
			Player &player, float deltaTime);
		virtual void updateSpecific(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
			Player &player, float deltaTime) {};

		virtual void walkPath(RunIn &runIn);
		virtual void boidCalculations(btVector3 &pos, btVector3 &dir,
			std::vector<Enemy*> const &close, float maxSpeed, float dt);

		void runTree(RunIn &in);
		
		void setRoot(NodeType type, int value, run func);
		BehaviorNode* addNode(BehaviorNode *parent, NodeType type, int value, run func);

		// Returns if current goal node has been changed and set it to false
		bool isGoalChangedAndSetToFalse();

		BehaviorNode* getRoot();
		Pathing& getPath();

		PathingType getPathingType() const;
		void setPathingType(PathingType pathingType);

        float getSteeringSpeed() const;
        void setSteeringSpeed(float steeringSpeed);
	private:
		PathingType m_pathingType;
	};
}

#endif
