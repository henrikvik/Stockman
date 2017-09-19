#ifndef PHYSICS_H
#define PHYSICS_H

/*
- Add rigid bodies into the world:
1. Make a btRigidBody object, add mass and stuff like that
2. Call physics->addRigidBody(ptr to rigidbody)
*/

#include <Entity\Entity.h>
#include <Physics\Primitives.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#define PHYSICS_GRAVITY 9.82

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

		btRigidBody* createBody(Cube& cube, float mass, bool isSensor);
		btRigidBody* createBody(Plane& plane, float mass, bool isSensor);
		btRigidBody* createBody(Sphere& sphere, float mass, bool isSensor);

	private:
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* constraintSolver;
		btDefaultCollisionConfiguration* collisionConfiguration;
	};
}


#endif // !PHYSICS_H
