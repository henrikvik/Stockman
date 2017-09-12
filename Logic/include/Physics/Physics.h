#ifndef PHYSICS_H
#define PHYSICS_H

/* 
 - Add rigid bodies into the world: 
  1. Make a btRigidBody object, add mass and stuff like that
  2. Call physics->addRigidBody(ptr to rigidbody)
 */

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#define PHYSICS_GRAVITY 9.82
#define PHYSICS_TIME_STEP 1.f / 60.f
#define PHYSICS_MAX_SUB_STEPS 10

namespace Logic
{

	class Physics : public btDiscreteDynamicsWorld
	{
	public:
		Physics(btCollisionDispatcher* dispatcher, btBroadphaseInterface* overlappingPairCache, btSequentialImpulseConstraintSolver* constraintSolver, btDefaultCollisionConfiguration* collisionConfiguration);
		~Physics();

		void clear();
		bool init();
		void update(float deltaTime);

	private:
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* constraintSolver;
		btDefaultCollisionConfiguration* collisionConfiguration;
	};
}


#endif // !PHYSICS_H
