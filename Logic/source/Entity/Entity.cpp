#include <Entity/Entity.h>

using namespace Logic;

Entity::Entity()
{
	m_body = nullptr;
	m_transform = nullptr;
}

Entity::~Entity() 
{
	// ALL physics is getting cleared by the Physics class, 
	//  including everything created with BulletPhysics in this class 
}

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
	case ShapeBox:			shape = new btBoxShape(bodyDesc.boxDimensions);
		break;
	case ShapeSphere:		shape = new btSphereShape(bodyDesc.radius);
		break;
	case ShapeStaticPlane:	shape = new btStaticPlaneShape(bodyDesc.normal, bodyDesc.scalar);
		break;
	default: shape = nullptr;
		break;
	}

	// Creating the actual body
	m_body = new btRigidBody(bodyDesc.mass, motionState, shape);

	// Specifics
	m_body->setRestitution(bodyDesc.restitution);	// Bounciness, 0:1 = Loses velocity with each bounce, < 1 = Gains velocity with each bounce
	m_body->setFriction(bodyDesc.friction);			// Friction, If set at zero, no spinning will happen

	// Connecting the bulletphysics world with the logic side
	m_body->setUserPointer(this);

	// Adding starting velocity
	m_body->applyForce(bodyDesc.velocity, bodyDesc.position);

	// Deactivates sleeping
	m_body->setSleepingThresholds(0, 0);

	// Adding body to the world
	physics->addRigidBody(m_body);

	// Saving the transform as a pointer for easy & optimized access 
	m_transform = &m_body->getWorldTransform();
}

void Logic::Entity::clear() { }

void Entity::update(float deltaTime)
{
	for (auto &effectPair : m_statusManager.getActiveEffects()) //opt
		affect(effectPair.first, *effectPair.second, deltaTime);
	m_statusManager.update(deltaTime);
	updateSpecific(deltaTime);
}

void Entity::collision(Entity& other)
{
	onCollision(other);
}

void Entity::affect(int stacks, Effect const &effect, float dt) {}

btRigidBody* Entity::getRigidbody()
{
	return m_body;
}

// JUST FOR TESTING, REMOVE
void Entity::consoleWritePosition()
{
	printf("Position = { %f, %f, %f }\n", m_transform->getOrigin().getX(), m_transform->getOrigin().getY(), m_transform->getOrigin().getZ());
}

DirectX::SimpleMath::Vector3 Entity::getPosition() const
{
	return DirectX::SimpleMath::Vector3(m_transform->getOrigin());
}

DirectX::SimpleMath::Quaternion Entity::getRotation() const
{
	return DirectX::SimpleMath::Quaternion(m_transform->getRotation());
}

DirectX::SimpleMath::Vector3 Entity::getScale() const
{
	return DirectX::SimpleMath::Vector3(m_body->getCollisionShape()->getLocalScaling());
}

DirectX::SimpleMath::Matrix Logic::Entity::getTransformMatrix() const
{
	// Making memory for a matrix
	float* m = new float[4 * 16];

	// Getting this entity's matrix
	m_transform->getOpenGLMatrix((btScalar*)(m));

	// Translating to DirectX Math and assigning the variables
	DirectX::SimpleMath::Matrix transformMatrix(m);

	// Deleting the old created variables from memory
	delete m;

	return transformMatrix;
}
