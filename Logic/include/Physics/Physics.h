#ifndef PHYSICS_H
#define PHYSICS_H

/* 
 * How to add rigid bodies into the world: 
  1. Make a btRigidBody object
  2. Set it up however you want
  3. Call physics->addRigidBody(ptr to rigidbody)

  (btRigidbody is derived from btCollisionObject)

 */

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

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
