#include "../../include/Entity/PhysicsObject.h"

using namespace Logic;

PhysicsObject::PhysicsObject(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID)
	: Object(modelID)
{
	if (body)
	{
		// Connecting Physics with Logic
		m_body = body;
		m_body->setUserPointer(this);

		// Saving halfextent
		m_halfextent = halfExtent;

		// Saving ptr to transform
		m_transform = &m_body->getWorldTransform();

		// Get the new transformation from bulletphysics and putting in graphics (for things that doesn't use the update loop things)
		setWorldTranslation(getTransformMatrix());
	}
}

PhysicsObject::~PhysicsObject()
{
	// Gets removed from memory in Physics
}

// Add an extra rigidbody connected to the main part
void PhysicsObject::addExtraBody(btRigidBody* body, float multiplier, btVector3 offset)
{
	body->setUserPointer(this);
	m_weakPoints.push_back(Weakpoint(body, multiplier, offset));
}

// Destroy this physics object early
void PhysicsObject::destroyBody()
{
	// Delete main body
	if (m_body)
	{
		if (m_body->getMotionState())
			delete m_body->getMotionState();
		if (m_body->getCollisionShape())
			delete m_body->getCollisionShape();
		delete m_body;
	}

	// Delete the weakpoints
	for (int i = 0; i < m_weakPoints.size(); i++)
	{
		btRigidBody* weakPoint = m_weakPoints[i].body;
		if (weakPoint->getMotionState())
			delete weakPoint->getMotionState();
		if (weakPoint->getCollisionShape())
			delete weakPoint->getCollisionShape();
		delete weakPoint;
	}
}

void PhysicsObject::updatePhysics(float deltaTime)
{
	// Updating the positions of the weakpoints
	if (!m_weakPoints.empty())
	{
		for (int i = 0; i < m_weakPoints.size(); i++)
		{
			Weakpoint weakPoint = m_weakPoints[i];

			float n[16];
			m_transform->getOpenGLMatrix((btScalar*)(&n));
			weakPoint.body->getWorldTransform().setFromOpenGLMatrix((btScalar*)(&n));
			weakPoint.body->getWorldTransform().setOrigin(m_transform->getOrigin() + btVector3(n[4] * weakPoint.offset.x(), n[5] * weakPoint.offset.y(), n[6] * weakPoint.offset.z()));
		}
	}

	// Get the new transformation from bulletphysics and putting in graphics
	setWorldTranslation(getTransformMatrix());
}

void PhysicsObject::collision(PhysicsObject & other, btVector3 contactPoint, const btRigidBody * collidedWithYour)
{
	// Checks if the collision happened on one of the weakpoints
	bool hit = false;
	if (!m_weakPoints.empty())
	{
		for (int i = 0; i < m_weakPoints.size(); i++)
		{
			Weakpoint weakPoint = m_weakPoints[i];

			if (collidedWithYour == weakPoint.body)
			{
				onCollision(other, contactPoint, weakPoint.multiplier);
				hit = true;
			}
		}
	}

	if (!hit)
		onCollision(other, contactPoint, 1.f);
}

btVector3 PhysicsObject::getHalfExtent() const
{
	return m_halfextent;
}

DirectX::SimpleMath::Vector3 PhysicsObject::getPosition() const
{
	return DirectX::SimpleMath::Vector3(m_transform->getOrigin());
}

btVector3 PhysicsObject::getPositionBT() const
{
	return m_transform->getOrigin();
}

DirectX::SimpleMath::Quaternion PhysicsObject::getRotation() const
{
	return DirectX::SimpleMath::Quaternion(m_transform->getRotation());
}

DirectX::SimpleMath::Vector3 PhysicsObject::getScale() const
{
	return DirectX::SimpleMath::Vector3(m_body->getCollisionShape()->getLocalScaling());
}

DirectX::SimpleMath::Matrix PhysicsObject::getTransformMatrix() const
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

btRigidBody* PhysicsObject::getRigidBody()
{
	return m_body;
}

btTransform& PhysicsObject::getTransform()
{
	return *m_transform;
}

int PhysicsObject::getNumberOfWeakPoints() const
{
	return m_weakPoints.size();
}

btRigidBody * PhysicsObject::getRigidBodyWeakPoint(int i)
{
	return m_weakPoints[i].body;
}
