#include <Entity/Entity.h>

using namespace Logic;

Entity::Entity(btRigidBody* body, btVector3 halfextent, Graphics::ModelID modelID)
: Object(modelID)
{
	// Saving ptr to rigidbody and connecting them to each other
	m_body = body;
	m_body->setUserPointer(this);

	// Setting the weakness body as nullptr
	m_bodyWeakPoint = nullptr;

	// Saving ptr to the transform for easier reach
	m_transform = &m_body->getWorldTransform();

	// Saving the size for graphics to draw the correct scale
	m_halfextent = halfextent;

	// For non moving object, that doesn't update loop
	updateGraphics();
}

Entity::~Entity() 
{
	// ALL physics is getting cleared by the Physics class, but you can delete an entity early by calling destroyBody() below
}

void Entity::addWeakpoint(btRigidBody* body, btVector3 offset)
{
	m_bodyWeakPoint = body;
	m_bodyWeakPoint->setUserPointer(this);
	m_weakPointOffset = offset;
}

void Entity::destroyBody()
{
	if (m_body)
	{
		if (m_body->getMotionState())
			delete m_body->getMotionState();
		if (m_body->getCollisionShape())
			delete m_body->getCollisionShape();
		delete m_body;
	}
	if (m_bodyWeakPoint)
	{
		if (m_bodyWeakPoint->getMotionState())
			delete m_bodyWeakPoint->getMotionState();
		if (m_bodyWeakPoint->getCollisionShape())
			delete m_bodyWeakPoint->getCollisionShape();
		delete m_bodyWeakPoint;
	}
}

void Entity::clear() { }

void Entity::update(float deltaTime)
{
	// Updating the position of the weakpoint
	if (m_bodyWeakPoint)
	{
		btVector3 mainOrigin = m_transform->getOrigin();
		btVector3 newOrigin = mainOrigin + m_weakPointOffset;
		m_bodyWeakPoint->getWorldTransform().setOrigin(newOrigin);
	}

	// Checking different buffs
	for (auto &effectPair : m_statusManager.getActiveEffects()) //opt
		affect(effectPair.first, *effectPair.second, deltaTime);
	
	// Updating every at
	m_statusManager.update(deltaTime);

	// Updating specific
	updateSpecific(deltaTime);

	// Updating the graphical sizde
	updateGraphics();
}

void Entity::updateGraphics()
{
	// Get the new transformation from bulletphysics
	setWorldTranslation(getTransformMatrix());
}

void Entity::collision(Entity& other, btVector3 contactPoint, const btRigidBody* collidedWithYour)
{
	// Remove this.
	if (collidedWithYour == m_bodyWeakPoint)
		printf("Auch. You hit my head, jerk.\n");

	onCollision(other, contactPoint, (collidedWithYour == m_bodyWeakPoint) ? 2.f : 1.f);
}

void Entity::affect(int stacks, Effect const &effect, float dt) {}

void Entity::upgrade(Upgrade const & upgrade) {}

btRigidBody* Entity::getRigidbody()
{
	return m_body;
}

StatusManager& Entity::getStatusManager()
{
	return m_statusManager;
}

void Entity::setHalfExtent(btVector3 halfExtent)
{
	m_halfextent = halfExtent;
}

btVector3 Entity::getHalfExtent() const
{
	return m_halfextent;
}

void Entity::setStatusManager(StatusManager & statusManager)
{
	m_statusManager = statusManager;
}

DirectX::SimpleMath::Vector3 Entity::getPosition() const
{
	return DirectX::SimpleMath::Vector3(m_transform->getOrigin());
}

btVector3 Entity::getPositionBT() const
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
