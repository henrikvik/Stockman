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
	this->setLatencyMotionStateInterpolation(false);

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

void Physics::update(GameTime gameTime)
{
	static std::chrono::steady_clock::time_point begin;
	static std::chrono::steady_clock::time_point end;

	// Calculate the time since last call and tell bulletphysics
	begin = std::chrono::steady_clock::now();
	float microsec = std::chrono::duration_cast<std::chrono::microseconds>(begin - end).count() * 0.000001;
	
	// Adding slowmotion effects
	microsec *= gameTime.currentMod;
	
	// Stepping the physics
	this->stepSimulation(microsec, 16);

	// Saving the end time
	end = std::chrono::steady_clock::now();
	
	// Collisions
	int numManifolds = dispatcher->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			Entity* pbodyA = reinterpret_cast<Entity*>(obA->getUserPointer());
			Entity* pbodyB = reinterpret_cast<Entity*>(obB->getUserPointer());

			if (pbodyA && pbodyB)
			{
				pbodyA->collision(*pbodyB);
				pbodyB->collision(*pbodyA);
			}
		}
	}
}

const btRigidBody* Logic::Physics::checkRayIntersect(Ray& ray)
{
	const btVector3& start	= ray.getStart();
	const btVector3& end	= ray.getEnd();

	// Ray testing to see first callback
	btCollisionWorld::ClosestRayResultCallback rayCallBack(start, end);
	this->rayTest(start, end, rayCallBack);

	if (rayCallBack.hasHit())
	{
		const btCollisionObject* object = rayCallBack.m_collisionObject;
		const btRigidBody* body = btRigidBody::upcast(object);;

		return body;
	}

	return nullptr;
}

btRigidBody* Physics::createBody(Cube& cube, float mass, bool isSensor)
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
	shape->setUserPointer(body);

	// If the body is a trigger
	if (isSensor)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Specifics
	body->setRestitution(0.0f);
	body->setFriction(1.0f);
	body->setSleepingThresholds(0, 0);

	// Adding body to the world
	this->addRigidBody(body);

	return body;
}

btRigidBody * Physics::createBody(Plane& plane, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(plane.getRot().getZ(), plane.getRot().getY(), plane.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, plane.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btStaticPlaneShape(plane.getNormal(), 0.1f);

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape);
	btRigidBody* body = new btRigidBody(constructionInfo);
	shape->setUserPointer(body);

	// If the body is a trigger
	if (isSensor)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Specifics
	body->setRestitution(0.0f);
	body->setFriction(1.0f);
	body->setSleepingThresholds(0, 0);

	// Adding body to the world
	this->addRigidBody(body);

	return body;
}

btRigidBody * Physics::createBody(Sphere& sphere, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(sphere.getRot().getZ(), sphere.getRot().getY(), sphere.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, sphere.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btSphereShape(sphere.getRadius());

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape);
	btRigidBody* body = new btRigidBody(constructionInfo);
	shape->setUserPointer(body);

	// If the body is a trigger
	if (isSensor)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Specifics
	body->setRestitution(0.0f);
	body->setFriction(1.f);
	body->setSleepingThresholds(0, 0);	
	body->setDamping(0.f, 0.f);

	// Adding body to the world
	this->addRigidBody(body);

	return body;
}

btRigidBody* Logic::Physics::createBody(Cylinder& cylinder, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(cylinder.getRot().getZ(), cylinder.getRot().getY(), cylinder.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, cylinder.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btCylinderShape(cylinder.getHalfExtends());

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape);
	btRigidBody* body = new btRigidBody(constructionInfo);
	shape->setUserPointer(body);

	// If the body is a trigger
	if (isSensor)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Specifics
	body->setRestitution(0.0f);
	body->setFriction(1.f);
	body->setSleepingThresholds(0, 0);
	body->setDamping(0.f, 0.f);

	// Making the cylinder a kinematic body
	//body->setCollisionFlags(body->getCollisionFlags() | btRigidBody::CF_KINEMATIC_OBJECT | btRigidBody::CF_STATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);

	// Adding body to the world
	this->addRigidBody(body);

	return body;
}

btRigidBody* Physics::createBody(Capsule& capsule, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(capsule.getRot().getZ(), capsule.getRot().getY(), capsule.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, capsule.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btCapsuleShape(capsule.getRadius(), capsule.getHeight());

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape);
	btRigidBody* body = new btRigidBody(constructionInfo);
	shape->setUserPointer(body);

	// If the body is a trigger
	if (isSensor)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Specifics
	body->setRestitution(0.0f);
	body->setFriction(0.f);
	body->setSleepingThresholds(0, 0);
	body->setDamping(0.0f, 0.0f);

	// Adding body to the world
	this->addRigidBody(body);

	return body;
}
