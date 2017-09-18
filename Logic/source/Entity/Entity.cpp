#include <Entity/Entity.h>

using namespace Logic;

Entity::Entity(btRigidBody* body)
{
	m_body = body;
	m_transform = &m_body->getWorldTransform();
}

Entity::~Entity() 
{
	// ALL physics is getting cleared by the Physics class, 
}

void Entity::clear() { }

void Entity::update(float deltaTime)
{
	for (auto &effectPair : m_statusManager.getActiveEffects()) //opt
		affect(effectPair.first, *effectPair.second, deltaTime);
	
	// Updating every at
	m_statusManager.update(deltaTime);

	// Updating specific
	updateSpecific(deltaTime);

	// Get the new transformation from bulletphysics
	setWorldMatrix(getTransformMatrix());
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

DirectX::SimpleMath::Matrix Entity::getTransformMatrix() const
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
