#include <Logic\include\Entity\GrapplingPoint.h>
#include "../../include/Player/Player.h"
#include "../../include/Physics/Physics.h"

using namespace Logic;

GrapplingPoint::GrapplingPoint(btRigidBody * body, btVector3 halfExtent, Graphics::ModelID modelID)
	: Entity(body, halfExtent, modelID) {
}

GrapplingPoint::~GrapplingPoint()
{
	m_physicsPtr = nullptr;
}

void GrapplingPoint::init(Physics* physics, Player* player)
{
	m_physicsPtr = physics;
	m_playerPtr = player;
}

void GrapplingPoint::onCollision(Entity& other)
{
	if (Projectile* p = dynamic_cast<Projectile*>(&other))
	{
		btRigidBody* playerBody = m_playerPtr->getRigidbody();
		btVector3 playerPos = m_playerPtr->getPositionBT();
		btVector3 pointPos = getPositionBT();

		const btRigidBody* intersectedBody = m_physicsPtr->checkRayIntersect(Ray(pointPos, playerPos));
		if (intersectedBody == playerBody)
		{
			playerBody->translate(pointPos - playerPos);
		}
		else if (intersectedBody == p->getRigidbody())
		{
			printf("The grappling-hook projectile was in the way of the ray-test.\n");
		}
		else
		{
			printf("Can't grapple with obstacles in the way.\n");
		}
	}
}
