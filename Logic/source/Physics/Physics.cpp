#include "Physics\Physics.h"

using namespace Logic;

Physics::Physics(btCollisionDispatcher* dispatcher, btBroadphaseInterface* overlappingPairCache, btSequentialImpulseConstraintSolver* constraintSolver, btDefaultCollisionConfiguration* collisionConfiguration)
	: btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration)
{
	this->dispatcher = dispatcher;
	this->overlappingPairCache = overlappingPairCache;
	this->constraintSolver = constraintSolver;
	this->collisionConfiguration = collisionConfiguration;
}

Physics::~Physics()
{
	clear();
}

bool Physics::init()
{
	// World gravity
	this->setGravity(btVector3(0, -PHYSICS_GRAVITY, 0));

	return true;
}

void Physics::clear()
{
	// Cleanup in the reverse order of creation / initialization!
	for (int i = this->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = this->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btCollisionShape* shape = obj->getCollisionShape();
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		this->removeCollisionObject(obj);
		delete shape;
		delete obj;
	}

	// Deleting members
	delete constraintSolver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
}

void Physics::update(float deltaTime)
{
	this->stepSimulation(deltaTime * 0.001f); // bulletphysics doesn't support a not constant framrate, calling this will make the game not g8
											  //	this->stepSimulation(1 / 60.f);
	int numManifolds = dispatcher->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			Entity* pbodyA = (Entity*)obA->getUserPointer();
			Entity* pbodyB = (Entity*)obB->getUserPointer();

			if (pbodyA && pbodyB)
			{
				pbodyA->collision(*pbodyB);
				pbodyB->collision(*pbodyA);
			}
		}
	}
}

Player* Logic::Physics::addPlayer(iCube& cube, float mass)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(cube.getRot().getZ(), cube.getRot().getY(), cube.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, cube.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btBoxShape(cube.getDimensions());
	btVector3 localInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, localInertia);

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape);
	btRigidBody* body = new btRigidBody(constructionInfo);
	Player* player = new Player(body);
	body->setUserPointer(player);

	// Specifics
	body->setRestitution(0.0f);		
	body->setFriction(0.1f);	
	body->setSleepingThresholds(0, 0);

	// Adding body to the world
	this->addRigidBody(body);

	return player;
}

Entity* Logic::Physics::addBody(iCube& cube, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(cube.getRot().getZ(), cube.getRot().getY(), cube.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, cube.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btBoxShape(cube.getDimensions());

	// Calculating the Inertia
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(constructionInfo);
	Entity* entity = new Entity(body);
	body->setUserPointer(entity);

	// Specifics
//	body->setRestitution(bodyDesc.restitution);		// Bounciness, 0:1 = Loses velocity with each bounce, < 1 = Gains velocity with each bounce
//	body->setFriction(bodyDesc.friction);			// Friction, If set at zero, no spinning will happen

	// Deactivates sleeping
	body->setSleepingThresholds(0, 0);

	// Adding body to the world
	this->addRigidBody(body);

	return entity;
}

Entity * Logic::Physics::addBody(iPlane& plane, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(plane.getRot().getZ(), plane.getRot().getY(), plane.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, plane.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btStaticPlaneShape(plane.getNormal(), 1);

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape);
	btRigidBody* body = new btRigidBody(constructionInfo);
	Entity* entity = new Player(body);
	body->setUserPointer(entity);

	// Specifics
	body->setRestitution(0.0f);
	body->setFriction(0.1f);
	body->setSleepingThresholds(0, 0);

	// Adding body to the world
	this->addRigidBody(body);

	return entity;
}

Entity * Logic::Physics::addBody(iSphere& sphere, float mass, bool isSensor)
{
	return nullptr;
}