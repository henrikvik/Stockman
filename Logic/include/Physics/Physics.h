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
#include <Engine\Profiler.h>

#define PHYSICS_GRAVITY 0.982f * 0.5f
//#define PHYSICS_GRAVITY 9.82f * 2.f

#define DEFAULT_F 0.5f
#define DEFAULT_R 0 
#define DEFAULT_S { 0, 0 }
#define DEFAULT_D { 0, 0 }

namespace Logic
{
	struct BodySpecifics
	{
		BodySpecifics(float restitution, float friction, DirectX::SimpleMath::Vector2 sleepingThresholds, DirectX::SimpleMath::Vector2 damping, bool isSensor)
			: isSensor(isSensor), restitution(restitution), friction(friction), sleepingThresholds(sleepingThresholds), damping(damping) { }

		bool isSensor;
		float restitution; 
		float friction;
		DirectX::SimpleMath::Vector2 sleepingThresholds;
		DirectX::SimpleMath::Vector2 damping;
	};

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
		btRigidBody* createBody(Cube& cube, float mass, bool isSensor = false);				// Should only be used for hitboxes on map, nothing else
		btRigidBody* createBody(Plane& plane, float mass, bool isSensor = false);			// Static infinite plane, keep this temporary
		btRigidBody* createBody(Sphere& sphere, float mass, bool isSensor = false);			// Should be used as often as possible because it needs less processing of collisions
		btRigidBody* createBody(Cylinder& cylinder, float mass, bool isSensor = false);		// Should be used for player & enemies
		btRigidBody* createBody(Capsule& capsule, float mass, bool isSensor = false);		// Should be used for player & enemies

		// Debug Rendering
		void render(Graphics::Renderer& renderer);

	private:
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* constraintSolver;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btRigidBody* initBody(btRigidBody::btRigidBodyConstructionInfo constructionInfo, BodySpecifics specifics);

		// Debug Rendering
		Graphics::RenderDebugInfo renderDebug;
		void renderCube(Graphics::Renderer& renderer, btBoxShape* bs, btRigidBody* body);
		void renderSphere(Graphics::Renderer& renderer, btSphereShape* ss, btRigidBody* body);
		void renderCylinder(Graphics::Renderer& renderer, btCylinderShape* cs, btRigidBody* body);
		void renderCapsule(Graphics::Renderer& renderer, btCapsuleShape* cs, btRigidBody* body);
		void renderRectangleAround(Graphics::Renderer& renderer, btVector3 origin, btVector3 half);
	};
}


#endif // !PHYSICS_H
