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

#include <Graphics\include\RenderQueue.h>

#include <ctime>
#include <chrono>
#include <Entity\PhysicsObject.h>
#include <Physics\Primitives.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>
#include <DebugWindow\Profiler.h>
#include <functional>


//#define PHYSICS_GRAVITY 0.00982f
#define PHYSICS_GRAVITY 9.82f * 2.f

#define DEFAULT_F 0.5f
#define DEFAULT_R 0 
#define DEFAULT_S { 0, 0 }
#define DEFAULT_D { 0, 0 }


namespace Logic
{
    class Physics : public btDiscreteDynamicsWorld
    {
    public:
        // Used as a builder/skeleton for a rigidbody
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

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //  Collision Flags *
        //
        //      These are used to make different types of objects ignore contact with each other
        //      * Example: A Box that only collides with Enemy Projectiles.
        //                  Group = COL_NOTHING_BUT_ENEMY_PROJECTILES (You have to create a new one, in this case)
        //                  Mask  = COL_EN_PROJ
        //                  RigidBody = (Cube, Mass, IsSensor, Group, Mask);         
        //
        enum COL_FLAG {
            COL_NOTHING = 0,
            COL_HITBOX  = 1 << 0,
            COL_PLAYER  = 1 << 1,
            COL_ENEMY   = 1 << 2,
            COL_EN_PROJ = 1 << 3,
            COL_PL_PROJ = 1 << 4,
        }; static const int COL_EVERYTHING = COL_HITBOX | COL_PLAYER | COL_ENEMY | COL_EN_PROJ | COL_PL_PROJ;
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        Physics(btCollisionDispatcher* dispatcher, btBroadphaseInterface* overlappingPairCache, btSequentialImpulseConstraintSolver* constraintSolver, btDefaultCollisionConfiguration* collisionConfiguration);
        ~Physics();
        void clear();
        bool init();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //  Physics Update *
        //      
        //      Stepping Physics at 60 frames per second
        //      Handling collision between PhysicObject's (\see PhysicsObject.h)
        //
        void update(float delta);

        void registerDebugCommands();									
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // Ray Testing * 
        // 
        //      Pretty Straight-Forward
        //      Can be used at anytime.
        //
        const btRigidBody*  RayTestOnRigidBodies(Ray& ray);
        const btVector3     RayTestGetPoint(Ray& ray);
        const btVector3     RayTestGetNormal(Ray& ray);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //  Create Body *
        //
        //      These functions are used to create RigidBodies.
        //      They are inserted in the Physics World instantly.
        //
        //      Where to use: In entity's constructor
        //      Note: CreatePlayer() is seperate because it's using a very different shape
        //
        btRigidBody* createBody(Shape* shape, float mass, bool isSensor = false, int group = COL_FLAG::COL_HITBOX, int mask = COL_EVERYTHING);          // More versitile function but more expensive
        btRigidBody* createBody(Cube& cube, float mass, bool isSensor = false, int group = COL_FLAG::COL_HITBOX, int mask = COL_EVERYTHING);			// Should only be used for hitboxes on map, nothing else
        btRigidBody* createBody(Plane& plane, float mass, bool isSensor = false, int group = COL_FLAG::COL_HITBOX, int mask = COL_EVERYTHING);			// Static infinite plane, should be used for testing only
        btRigidBody* createBody(Sphere& sphere, float mass, bool isSensor = false, int group = COL_FLAG::COL_HITBOX, int mask = COL_EVERYTHING);		// Should be used as often as possible because it needs less processing of collisions
        btRigidBody* createBody(Cylinder& cylinder, float mass, bool isSensor = false, int group = COL_FLAG::COL_HITBOX, int mask = COL_EVERYTHING);	// Should be used for enemies
        btRigidBody* createBody(Capsule& capsule, float mass, bool isSensor = false, int group = COL_FLAG::COL_HITBOX, int mask = COL_EVERYTHING);		// Should be used for enemies
        btPairCachingGhostObject* createPlayer(btCapsuleShape* capsule, btVector3 pos);

        btRigidBody* createHitbox(btVector3 position, btQuaternion rotation, btVector3 halfSize, bool isSensor, int group, int mask);

		// Debug Rendering
		void render();

    private:
        btCollisionDispatcher*                  dispatcher;
        btBroadphaseInterface*                  overlappingPairCache;
        btSequentialImpulseConstraintSolver*    constraintSolver;
        btDefaultCollisionConfiguration*        collisionConfiguration;
        btGhostPairCallback*                    ghostPairCB;

        btRigidBody* initBody(btRigidBody::btRigidBodyConstructionInfo constructionInfo, BodySpecifics specifics);

		// Debug Rendering
	    DebugRenderInfo debugRenderInfo;
		void renderCube(btBoxShape* bs, btRigidBody* body);
		void renderSphere(btSphereShape* ss, btRigidBody* body);
		void renderCylinder(btCylinderShape* cs, btRigidBody* body);
		void renderCapsule(btCapsuleShape* cs, btRigidBody* body);
		void renderRectangleAround(btVector3 origin, btVector3 half);
		void renderGhostCapsule(btCapsuleShape* cs, btGhostObject* ghostObject);
	};
}


#endif // !PHYSICS_H