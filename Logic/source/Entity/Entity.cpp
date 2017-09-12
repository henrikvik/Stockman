#include <Entity/Entity.h>

using namespace Logic;

Entity::Entity()
{
}

Entity::~Entity() { }

<<<<<<< HEAD
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
	m_statusManager.update(deltaTime);
	updateSpecific(deltaTime);
}

void Entity::collision(Entity& other)
=======
void Entity::collision(const Entity& other)
>>>>>>> logic
{
	onCollision(other);
}