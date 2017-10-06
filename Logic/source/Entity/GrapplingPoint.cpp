#include <Logic\include\Entity\GrapplingPoint.h>
#include "../../include/Player/Player.h"
#include "../../include/Physics/Physics.h"

using namespace Logic;

// Don't forget to init
GrapplingPoint::GrapplingPoint(btRigidBody * body, btVector3 halfExtent, Graphics::ModelID modelID)
	: Entity(body, halfExtent, modelID) 
{
	m_halfExtentNormal = halfExtent;
	m_correctAim = false;
	m_invisBox = nullptr;
}

GrapplingPoint::~GrapplingPoint()
{
	m_physicsPtr = nullptr;
}

// Creates the invisible box with physics, saves both pointers for later use
void GrapplingPoint::init(Physics* physics, Player* player)
{
	m_physicsPtr = physics;
	m_playerPtr = player;

	// Collects data from current rigidbody
	btRigidBody*	rb	= getRigidbody();
	btTransform		tf	= rb->getWorldTransform();
	btSphereShape*	ss	= dynamic_cast<btSphereShape*>(rb->getCollisionShape());
	float			r	= ((ss) ? ss->getRadius() : GP_INVIS_DEFAULT_RADIUS) * GP_INVIS_MOD;

	// Creates the invisiable rigidbody to handle indicator
//	m_invisBox	= physics->createBody(Sphere(tf.getOrigin(), GP_INVIS_ROT, r), GP_INVIS_MASS, true);
}

// Raytracing from the player towards the aiming area
void GrapplingPoint::updateSpecific(float deltaTime)
{
	static int frame = NULL;
	frame++;
	
	if (frame % GP_RAY_TRACE_FRAME)
	{
		btRigidBody* pointBody = getRigidbody();
		const btRigidBody* intersectedBody = m_physicsPtr->RayTestOnRigidBodies(Ray(m_playerPtr->getPositionBT(), m_playerPtr->getForwardBT(), GP_RAY_TRACE_DISTANCE));
		
		m_correctAim = (intersectedBody == pointBody);
		setModelID(m_correctAim ? Graphics::CUBE : Graphics::SPHERE);
	}
}

// Checks for collision with the player's grappling hook projectile
void GrapplingPoint::onCollision(Entity& other)
{
	if (Projectile* p = dynamic_cast<Projectile*>(&other))
	{
		if (p->getProjectileData().type != ProjectileType::ProjectileTypeGrappling)
			return;

		btRigidBody* playerBody = m_playerPtr->getRigidbody();
		btVector3 playerPos = m_playerPtr->getPositionBT();
		btVector3 pointPos = getPositionBT();

		const btRigidBody* intersectedBody = m_physicsPtr->RayTestOnRigidBodies(Ray(pointPos, playerPos));
		if (intersectedBody == p->getRigidbody() || intersectedBody == playerBody)
		{
			btVector3 dir = pointPos - playerPos;
			btVector3 dirY(NULL, GP_POWER * dir.y(), NULL);

			playerBody->applyCentralImpulse({ dir * GP_POWER });
			if (m_playerPtr->getMoveSpeed() < 0.001f)
			{
				m_playerPtr->setMoveSpeed(0.02f);
			}
			else
			{
				m_playerPtr->setMoveSpeed(m_playerPtr->getMoveSpeed() * 1.05f);
			}

			dir.normalize();
			btVector3 dirXZ(dir.x(), 0.f, dir.z());
			m_playerPtr->setMoveDirection({ dir.x(), NULL, dir.z() });
		}
		else if (intersectedBody == p->getRigidbody())
		{
			// Yeah, this should't be allowed happen, fix this
			printf("The grappling-hook projectile was in the way of the ray-test.\n");
		}
		else
		{
			printf("Can't grapple with obstacles in the way.\n");
		}
	}
}
