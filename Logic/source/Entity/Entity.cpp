#include <Entity/Entity.h>

using namespace Logic;

Entity::Entity(btRigidBody* body, btVector3 halfextent, Graphics::ModelID modelID)
: Object(modelID)
{
	m_body = body;
	m_body->setUserPointer(this);
	m_transform = &m_body->getWorldTransform();
	m_halfextent = halfextent;

	// For non moving object, that doesn't update loop
	updateGraphics();
}

Entity::~Entity() 
{
	// ALL physics is getting cleared by the Physics class, but you can delete an entity early by calling destroyBody() below
}

void Entity::destroyBody()
{
	if (m_body)
	{
		if (m_body->getMotionState())
			delete m_body->getMotionState();
		if(m_body->getCollisionShape())
			delete m_body->getCollisionShape();
		delete m_body;
	}
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

	updateGraphics();
}

void Entity::updateGraphics()
{
	// Get the new transformation from bulletphysics
	setWorldTranslation(getTransformMatrix());
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

StatusManager& Entity::getStatusManager()
{
	return m_statusManager;
}

DirectX::SimpleMath::Vector3 Entity::getPosition() const
{
	return DirectX::SimpleMath::Vector3(m_transform->getOrigin());
}

btVector3 Logic::Entity::getPositionBT() const
{
	return m_transform->getOrigin();
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

	//Find the scaling matrix
	auto scale = DirectX::SimpleMath::Matrix::CreateScale(m_halfextent.getX() * 2, m_halfextent.getY() * 2, m_halfextent.getZ() * 2);

	// Deleting the old created variables from memory
	delete m;

	return scale * transformMatrix;
}
