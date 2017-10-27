#include "Physics\Physics.h"
#include <Graphics\include\Renderer.h>


using namespace Logic;

#include <libs\Bullet2.86\include\BulletCollision\CollisionDispatch\btGhostObject.h>

Physics::Physics(btCollisionDispatcher* dispatcher, btBroadphaseInterface* overlappingPairCache, btSequentialImpulseConstraintSolver* constraintSolver, btDefaultCollisionConfiguration* collisionConfiguration)
	: btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration)
{
	this->dispatcher = dispatcher;
	this->overlappingPairCache = overlappingPairCache;
	this->constraintSolver = constraintSolver;
	this->collisionConfiguration = collisionConfiguration;

	// Render Debug Construction
	renderDebug.points = new std::vector<DirectX::SimpleMath::Vector3>();
	renderDebug.color = DirectX::SimpleMath::Color(1, 1, 1);
	renderDebug.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	renderDebug.useDepth = true;
}

Physics::~Physics()
{
	delete renderDebug.points;
	clear();
	delete ghostPairCB;
}

bool Physics::init()
{
	// World gravity
	this->setGravity(btVector3(0, -PHYSICS_GRAVITY, 0));
	this->setLatencyMotionStateInterpolation(true);
	ghostPairCB = new btGhostPairCallback();
	m_broadphasePairCache->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCB);
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
	// Stepping the physics
	PROFILE_BEGIN("Stepping Physics");

	if (gameTime.dtReal * 0.001f > (1.f / 60.f))
		this->stepSimulation(1.f / 60.f, 0, 0);
	else
		this->stepSimulation(gameTime.dtReal * 0.001f, 0, 0);

	PROFILE_END();

	PROFILE_BEGIN("Collision Handling");

	// Collisions
	int numManifolds = dispatcher->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);
	
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0) // Only returns the first contact as for now, fix this
		{
	//		if (obA->getCollisionShape()->getShapeType() != BroadphaseNativeTypes::SPHERE_SHAPE_PROXYTYPE)
		
			btManifoldPoint contactPoint = contactManifold->getContactPoint(0);

			btVector3 b = contactPoint.getPositionWorldOnA();
			btVector3 a = contactPoint.getPositionWorldOnB();

			PhysicsObject* entityA = reinterpret_cast<PhysicsObject*>(obA->getUserPointer());
			PhysicsObject* entityB = reinterpret_cast<PhysicsObject*>(obB->getUserPointer());

			const btRigidBody* rbodyA = btRigidBody::upcast(obA);
			const btRigidBody* rbodyB = btRigidBody::upcast(obB);

			if (entityA && entityB)
			{
				entityA->collision(*entityB, a, rbodyA);
				entityB->collision(*entityA, b, rbodyB);
			}
		}
	}
	PROFILE_END();
}

// Returns nullptr if not intersecting, otherwise returns the rigidbody of the hit
const btRigidBody* Physics::RayTestOnRigidBodies(Ray& ray)
{
	const btVector3& start	= ray.getStart();
	const btVector3& end	= ray.getEnd();

	// Ray testing to see first callback
	btCollisionWorld::ClosestRayResultCallback rayCallBack(start, end);
	this->rayTest(start, end, rayCallBack);

	if (rayCallBack.hasHit())
	{
		const btCollisionObject* object = rayCallBack.m_collisionObject;
		const btRigidBody* body = btRigidBody::upcast(object);

		return body;
	}

	return nullptr;
}

// Returns the actual point of intersection, returns { 0, 0, 0 } if not hit (I KNOW, IT BECOMES FUCKING STUPID IF YOU ACTUALLY HIT THE 0,0,0, BUT I DIDN''T FIND A SIMPLE WAY)
const btVector3 Physics::RayTestGetPoint(Ray & ray)
{
	const btVector3& start = ray.getStart();
	const btVector3& end = ray.getEnd();

	// Ray testing to see first callback
	btCollisionWorld::ClosestRayResultCallback rayCallBack(start, end);
	this->rayTest(start, end, rayCallBack);

	if (rayCallBack.hasHit())
	{
		const btVector3 object = rayCallBack.m_hitPointWorld;
		return object;
	}

	return { 0, 0, 0 };
}

const btVector3 Physics::RayTestGetNormal(Ray & ray)
{
	const btVector3& start = ray.getStart();
	const btVector3& end = ray.getEnd();

	// Ray testing to see first callback
	btCollisionWorld::ClosestRayResultCallback rayCallBack(start, end);
	this->rayTest(start, end, rayCallBack);

	if (rayCallBack.hasHit())
	{
		const btVector3 object = rayCallBack.m_hitNormalWorld;
		return object;
	}

	return { 0, 0, 0 };
}

btRigidBody* Physics::createBody(Shape* shape, float mass, bool isSensor)
{
    btRigidBody* body = nullptr;

    switch (shape->getType())
    {
    case ShapeType::ShapeTypeCube:      body = createBody(static_cast<Cube&>       (*shape), mass, isSensor); break;
    case ShapeType::ShapeTypeCapsule:   body = createBody(static_cast<Capsule&>    (*shape), mass, isSensor); break;
    case ShapeType::ShapeTypeCylinder:  body = createBody(static_cast<Cylinder&>   (*shape), mass, isSensor); break;
    case ShapeType::ShapeTypePlane:     body = createBody(static_cast<Plane&>      (*shape), mass, isSensor); break;
    case ShapeType::ShapeTypeSphere:    body = createBody(static_cast<Sphere&>     (*shape), mass, isSensor); break;
    default: printf("Could not create rigidbody, what the fuck did you do?\n"); break;
    }

    return body;
}

btRigidBody* Physics::createBody(Cube& cube, float mass, bool isSensor)
{
	// Setting Motions state with position & rotation
	btQuaternion rotation;
	rotation.setEulerZYX(cube.getRot().getZ(), cube.getRot().getY(), cube.getRot().getX());
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, cube.getPos()));

	// Creating the specific shape
	btCollisionShape* shape = new btBoxShape(cube.getDimensions());
	
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	// Creating the actual body
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, localInertia);
	BodySpecifics specifics(DEFAULT_R, DEFAULT_F, DEFAULT_S, DEFAULT_D, isSensor);
	btRigidBody* body = initBody(constructionInfo, specifics);
	shape->setUserPointer(body);

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
	BodySpecifics specifics(DEFAULT_R, DEFAULT_F, DEFAULT_S, DEFAULT_D, isSensor);
	btRigidBody* body = initBody(constructionInfo, specifics);
	shape->setUserPointer(body);

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
	BodySpecifics specifics(DEFAULT_R, DEFAULT_F, DEFAULT_S, DEFAULT_D, isSensor);
	btRigidBody* body = initBody(constructionInfo, specifics);
	shape->setUserPointer(body);

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
	BodySpecifics specifics(DEFAULT_R, DEFAULT_F, DEFAULT_S, DEFAULT_D, isSensor);
	btRigidBody* body = initBody(constructionInfo, specifics);
	shape->setUserPointer(body);

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
	BodySpecifics specifics(DEFAULT_R, DEFAULT_F, DEFAULT_S, DEFAULT_D, isSensor);
	btRigidBody* body = initBody(constructionInfo, specifics);
	shape->setUserPointer(body);

	// Adding body to the world
	this->addRigidBody(body);

	return body;
}

btPairCachingGhostObject* Physics::createPlayer(btCapsuleShape* capsule, btVector3 pos)
{
	btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();

	ghostObject->setCollisionShape(capsule);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// Rotation
	btQuaternion rotation;
	rotation.setEulerZYX(0.f, 0.f, 0.f);
	ghostObject->getWorldTransform().setRotation(rotation);

	// Position
	btTransform transform = ghostObject->getWorldTransform();
	transform.setOrigin(pos);
	ghostObject->setWorldTransform(transform);

	// Adding to physics world
	this->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	return ghostObject;
}

// Only used for debugging, draws all collision shapes onto screen
void Physics::render(Graphics::Renderer & renderer)
{
	// Clearing last debug draw
	renderDebug.points->clear();

	for (int i = this->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = this->getCollisionObjectArray()[i];
		if (btGhostObject* ghostObject = dynamic_cast<btGhostObject*>(obj))
		{
			renderGhostCapsule(renderer, dynamic_cast<btCapsuleShape*>(ghostObject->getCollisionShape()), ghostObject);
		}
		else
		{
			btRigidBody* body = btRigidBody::upcast(obj);
			btCollisionShape* shape = obj->getCollisionShape();

			// Render Boxes
			if (btBoxShape* bs = dynamic_cast<btBoxShape*>(shape))
				renderCube(renderer, bs, body);

			// Render Spheres
			else if (btSphereShape* ss = dynamic_cast<btSphereShape*>(shape))
				renderSphere(renderer, ss, body);

			// Render Cylinders
			else if (btCylinderShape* cs = dynamic_cast<btCylinderShape*>(shape))
				renderCylinder(renderer, cs, body);

			// Render Capsules
			else if (btCapsuleShape* cs = dynamic_cast<btCapsuleShape*>(shape))
				renderCapsule(renderer, cs, body);
		}
	}
}

btRigidBody* Physics::initBody(btRigidBody::btRigidBodyConstructionInfo constructionInfo, BodySpecifics specifics)
{
	btRigidBody* body = new btRigidBody(constructionInfo);

	// If the body is a trigger
	if (specifics.isSensor)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Specifics
	body->setRestitution(specifics.restitution);
	body->setFriction(specifics.friction);
	body->setSleepingThresholds(specifics.sleepingThresholds.x, specifics.sleepingThresholds.y);
	body->setDamping(specifics.damping.x, specifics.damping.y);

	return body;
}

// Render a debug with help of it's vertices
void Physics::renderCube(Graphics::Renderer& renderer, btBoxShape* bs, btRigidBody* body)
{
	btVector3 vp = { 0, 0, 0 };
	btVector3 center = body->getWorldTransform().getOrigin();
	btQuaternion q = body->getWorldTransform().getRotation();
	DirectX::SimpleMath::Matrix quaternion = DirectX::SimpleMath::Matrix::CreateFromQuaternion(DirectX::SimpleMath::Quaternion(q));

	// Front side
	for (int i = 0; i < bs->getNumVertices() - 1; i++)
	{
		bs->getVertex(0, vp);
		renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
		bs->getVertex(i + 1, vp);
		renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));

		bs->getVertex(bs->getNumVertices() - 1, vp);
		renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
		bs->getVertex(i + 1, vp);
		renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	}


	// Diagonal right
	bs->getVertex(5, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(4, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(5, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(1, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));

	// Diagonal left
	bs->getVertex(6, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(4, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(6, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(2, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));

	// Diagonal top
	bs->getVertex(3, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(1, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(3, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));
	bs->getVertex(2, vp);
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(center) + DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(vp), quaternion));

	renderer.queueRenderDebug(&renderDebug);
}

// Draws a cube around the sphere
void Physics::renderSphere(Graphics::Renderer& renderer, btSphereShape* ss, btRigidBody* body)
{
	btVector3 origin = body->getWorldTransform().getOrigin();
	btVector3 c;
	float r = 0.f;
	ss->getBoundingSphere(c, r);
	r /= 2;

	renderRectangleAround(renderer, origin, {r, r, r});
}

// Draws a cube around the cylinder
void Physics::renderCylinder(Graphics::Renderer & renderer, btCylinderShape * cs, btRigidBody * body)
{
	btVector3 origin = body->getWorldTransform().getOrigin();
	btVector3 half = cs->getHalfExtentsWithMargin();

	renderRectangleAround(renderer, origin, half);
}

// Draws a cube around the capsule
void Physics::renderCapsule(Graphics::Renderer& renderer, btCapsuleShape* cs, btRigidBody* body)
{
	btVector3 origin = body->getWorldTransform().getOrigin();
	btVector3 half = cs->getImplicitShapeDimensions();

	renderRectangleAround(renderer, origin, half);
}

// Draws a cube around the capsule
void Physics::renderGhostCapsule(Graphics::Renderer& renderer, btCapsuleShape* cs, btGhostObject* ghostObject)
{
	btVector3 origin = ghostObject->getWorldTransform().getOrigin();
	btVector3 half = cs->getImplicitShapeDimensions();

	renderRectangleAround(renderer, origin, half);
}

void Physics::renderRectangleAround(Graphics::Renderer& renderer, btVector3 origin, btVector3 half)
{
	// Side Front
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() + half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() + half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() - half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() - half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() + half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() - half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() + half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() - half.y(), origin.z() - half.z()));

	// Side Back
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() + half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() + half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() - half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() - half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() + half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() - half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() + half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() - half.y(), origin.z() - half.z()));

	// Left
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() + half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() + half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() - half.y(), origin.z() + half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() - half.y(), origin.z() + half.z()));

	// Right
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() + half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() + half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() + half.x(), origin.y() - half.y(), origin.z() - half.z()));
	renderDebug.points->push_back(DirectX::SimpleMath::Vector3(origin.x() - half.x(), origin.y() - half.y(), origin.z() - half.z()));

	renderer.queueRenderDebug(&renderDebug);
}
