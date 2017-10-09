#ifndef PHYSICS_H
#define PHYSICS_H

#pragma region ClassDesc
	/*
	CLASS: Physics
	AUTHOR: Simon

	DESCRIPTION: This class handles all the physics in the game.
				
				It creates all the rigidbodies & removes them from memory.


	HOW TO USE:
		- How to create a Entity with physics.

		//  Creating a body with a rigidbody as a CUBE rigidbody.
		{
			// Check Primitives for the different shapes
			Cube cube (pos, rot, size);

			// The createBody function will return a pointer to the created rigidbody
			btRigidBody* body = physics.createBody(cube, mass, isSensor);
			
			// Connect the body with the entity
			Entity* entity(body);
		}

		// Can be shortened down to one row if wanted
		{
			Entity* entity = (physics.createBody(Cube(pos, rot, size), mass, isSensor));
		}

	*/
#pragma endregion Comment

#include <ctime>
#include <chrono>
#include <Entity\PhysicsObject.h>
#include <Physics\Primitives.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Misc\GameTime.h>

#define PHYSICS_GRAVITY 9.82f * 2.f

namespace Logic
{
	class Physics : public btDiscreteDynamicsWorld
	{
	public:
		Physics(btCollisionDispatcher* dispatcher, btBroadphaseInterface* overlappingPairCache, btSequentialImpulseConstraintSolver* constraintSolver, btDefaultCollisionConfiguration* collisionConfiguration);
		~Physics();

		void clear();
		bool init();
		void update(GameTime gameTime);

		const btRigidBody* RayTestOnRigidBodies(Ray& ray);
		const btVector3 RayTestGetPoint(Ray& ray);
		const btVector3 RayTestGetNormal(Ray& ray);

		// Returns a ptr to the created rigidbody
		// Works with different primitives
		// Mass is the weight, for simplicity, use it like kilogram
		// Sensor is used for triggers and will not interact with other rigidbodies
		//	but it will tell us if it collides with anything
		btRigidBody* createBody(Cube& cube, float mass, bool isSensor = false);
		btRigidBody* createBody(Plane& plane, float mass, bool isSensor = false);			// Static infinite plane, keep this temporary
		btRigidBody* createBody(Sphere& sphere, float mass, bool isSensor = false);			
		btRigidBody* createBody(Cylinder& cylinder, float mass, bool isSensor = false);
		btRigidBody* createBody(Capsule& capsule, float mass, bool isSensor = false);		// Should be used for player & enemies

		void render(Graphics::Renderer& renderer);

	private:
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* constraintSolver;
		btDefaultCollisionConfiguration* collisionConfiguration;

		// Debug Rendering
		Graphics::RenderDebugInfo renderDebug;

		void renderCube(Graphics::Renderer& renderer, btBoxShape* bs, btRigidBody* body);
		void renderSphere(Graphics::Renderer& renderer, btSphereShape* ss, btRigidBody* body);
		btRigidBody* initBody(btRigidBody::btRigidBodyConstructionInfo constructionInfo, bool isSensor);
	};
}


#endif // !PHYSICS_H
