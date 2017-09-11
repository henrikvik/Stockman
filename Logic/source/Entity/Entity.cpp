#include "Entity/Entity.h"

using namespace Logic;

Entity::Entity()
{
	m_rigidBody = nullptr;
}

Entity::~Entity() { }

bool Entity::init(Physics* physics, RigidBodyDesc rigidBodyDesc)
{
	if (physics == nullptr)
		return false;

	// Setting rotation & position
	btQuaternion rotation;
	rotation.setEulerZYX(rigidBodyDesc.rotation.getZ(), rigidBodyDesc.rotation.getY(), rigidBodyDesc.rotation.getX());

	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, rigidBodyDesc.position));

	// Creating the specific shape
	btCollisionShape* shape;
	switch (rigidBodyDesc.shape)
	{
	case ShapeSphere:
		shape = new btSphereShape(rigidBodyDesc.radius);
		break;
	case ShapeRectangle:
		shape = new btBoxShape(rigidBodyDesc.boxDimensions);
		break;
	default: // Function will return false, must be a selected shape
		shape = nullptr;
		return false;
		break;
	}

	// Creating the actual body
	m_rigidBody = new btRigidBody(rigidBodyDesc.mass, motionState, shape);
	
	// Specifics
	m_rigidBody->setRestitution(rigidBodyDesc.restitution);	// Bounciness, 0:1 = Loses velocity with each bounce, < 1 = Gains velocity with each bounce
	m_rigidBody->setFriction(rigidBodyDesc.friction);		// Friction, If set at zero, no spinning will happen

	// Connecting the bulletphysics world with ours
	m_rigidBody->setUserPointer(this);

	// Adding starting velocity
	m_rigidBody->applyForce(rigidBodyDesc.velocity, rigidBodyDesc.position);

	// Adding body to the world
	physics->addRigidBody(m_rigidBody); 

	return true;
}

void Entity::update(float deltaTime)
{
	updateSpecific(deltaTime);
}

void Entity::collision(Entity& other)
{
	onCollision(other);
}

// JUST FOR TESTING, REMOVE
void Entity::consoleWritePosition()
{
	btTransform trans;
	m_rigidBody->getMotionState()->getWorldTransform(trans);

	printf("%f\n", trans.getOrigin().getY());
}