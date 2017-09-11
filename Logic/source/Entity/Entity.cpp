#include "Entity/Entity.h"

using namespace Logic;

Entity::Entity()
{
	m_body = nullptr;
}

Entity::~Entity() { }

bool Entity::init(Physics* physics, BodyDesc bodyDesc)
{
	if (physics == nullptr)
		return false;

	createBody(physics, bodyDesc);

	return true;
}

void Entity::createBody(Physics* physics, BodyDesc bodyDesc)
{
	// Setting rotation & position
	btQuaternion rotation;
	rotation.setEulerZYX(bodyDesc.rotation.getZ(), bodyDesc.rotation.getY(), bodyDesc.rotation.getX());

	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, bodyDesc.position));

	// Creating the specific shape
	btCollisionShape* shape;
	switch (bodyDesc.shape)
	{
	case ShapeBox:
		shape = new btBoxShape(bodyDesc.boxDimensions);
		break;
	case ShapeSphere:
		shape = new btSphereShape(bodyDesc.radius);
		break;
	case ShapeStaticPlane:
		shape = new btStaticPlaneShape(bodyDesc.normal, bodyDesc.scalar);
		break;
	default: 
		shape = nullptr;
		break;
	}

	// Creating the actual body
	m_body = new btRigidBody(bodyDesc.mass, motionState, shape);

	// Specifics
	m_body->setRestitution(bodyDesc.restitution);	// Bounciness, 0:1 = Loses velocity with each bounce, < 1 = Gains velocity with each bounce
	m_body->setFriction(bodyDesc.friction);		// Friction, If set at zero, no spinning will happen

	// Connecting the bulletphysics world with the logic side
	m_body->setUserPointer(this);

	// Adding starting velocity
	m_body->applyForce(bodyDesc.velocity, bodyDesc.position);

	// Adding body to the world
	physics->addRigidBody(m_body);
}

void Entity::update(float deltaTime)
{
	updateSpecific(deltaTime);
}

void Entity::collision(Entity& other)
{
	onCollision(other);
}

btRigidBody* Logic::Entity::getRigidbody()
{
	return m_body;
}

// JUST FOR TESTING, REMOVE
void Entity::consoleWritePosition()
{
	btTransform trans;
	m_body->getMotionState()->getWorldTransform(trans);

	printf("Position = { %f, %f, %f }\n", trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}