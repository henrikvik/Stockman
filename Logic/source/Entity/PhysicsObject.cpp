#include "../../include/Entity/PhysicsObject.h"
#include <Physics/Physics.h>
#include <Projectile\Projectile.h>
#include <Player\Player.h>
#include <Engine\newd.h>

using namespace Logic;

PhysicsObject::PhysicsObject(btRigidBody* body, btVector3 halfExtent, btVector3 modelOffset)
{
	if (body)
	{
		// Connecting Physics with Logic
		m_body = body;
		m_body->setUserPointer(this);

		// Saving halfextent
		m_halfextent = halfExtent;
        m_modelOffset = modelOffset;

		// Saving ptr to transform
		m_transform = &m_body->getWorldTransform();
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
}

void PhysicsObject::collision(PhysicsObject & other, btVector3 contactPoint, Physics &physics)
{
	// Checks if the collision happened on one of the weakpoints
	bool hit = false;
	if (!m_weakPoints.empty())
	{
        Projectile* projectile = dynamic_cast<Projectile*>(&other);
        // Check if other == projectile and not melee
        if (projectile && projectile->getProjectileData().type != ProjectileTypeMelee)
        {
            for (int i = 0; i < m_weakPoints.size(); i++)   
            {
                Weakpoint weakPoint = m_weakPoints[i];

                FunContactResult res(
                    [&](btBroadphaseProxy* proxy) -> bool {
                    return true;
                },
                    [&](btManifoldPoint& cp,
                        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                        const btCollisionObjectWrapper* colObj1, int partId1, int index1) -> btScalar
                {
                    if (cp.getDistance() < 0.05f)
                    {
                        onCollision(other, contactPoint, weakPoint.multiplier);
                        hit = true;
                    }
                    return 0;
                });
                physics.contactPairTest(weakPoint.body, other.getRigidBody(), res);
            }
        }
	}

	if (!hit)
		onCollision(other, contactPoint, 1.f);
}

void Logic::PhysicsObject::setModelOffset(btVector3 modelOffset)
{
    m_modelOffset = modelOffset;
}

void Logic::PhysicsObject::setHalfExtent(btVector3 halfExtent)
{
    m_halfextent = halfExtent;
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
    //btVector3 pos = m_transform->getOrigin();
    //m_transform->setOrigin(pos + m_modelOffset);

	// Making memory for a matrix
	float m[16];
	m_transform->getOpenGLMatrix((btScalar*)(m));

	// Translating to DirectX Math and assigning the variables
	DirectX::SimpleMath::Matrix transformMatrix(m);

	//Find the scaling matrix
	auto scale = DirectX::SimpleMath::Matrix::CreateScale(m_halfextent.getX() * 2, m_halfextent.getY() * 2, m_halfextent.getZ() * 2);

	return scale * transformMatrix;

    //m_transform->setOrigin(pos);
}

DirectX::SimpleMath::Matrix PhysicsObject::getModelTransformMatrix() const
{
    btTransform trans = btTransform(*m_transform);
    trans.setOrigin(trans.getOrigin() + m_modelOffset);

    // Making memory for a matrix
    float m[16];
    trans.getOpenGLMatrix((btScalar*)(m));

    // Translating to DirectX Math and assigning the variables
    DirectX::SimpleMath::Matrix transformMatrix(m);

    //Find the scaling matrix
    auto scale = DirectX::SimpleMath::Matrix::CreateScale(m_halfextent.getX() * 2, m_halfextent.getY() * 2, m_halfextent.getZ() * 2);

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
	return (int)m_weakPoints.size();
}

btRigidBody * PhysicsObject::getRigidBodyWeakPoint(int i)
{
	return m_weakPoints[i].body;
}
