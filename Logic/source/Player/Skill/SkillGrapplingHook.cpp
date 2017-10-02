#include "Player/Skill/SkillGrapplingHook.h"
#include <Player\Player.h>

using namespace Logic;

SkillGrapplingHook::SkillGrapplingHook(Physics* physics)
: Skill(GRAPPLING_HOOK_CD)
{
	m_physicsPtr = physics;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };
}

SkillGrapplingHook::~SkillGrapplingHook()
{
	m_physicsPtr = nullptr;
	m_shooter = nullptr;
}

void SkillGrapplingHook::onUse(btVector3 forward, Entity& shooter)
{
	const btRigidBody* intersection = m_physicsPtr->RayTestOnRigidBodies(Ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE));
	if (intersection)
	{
		printf("Hit! - Something else\n");
		if (Entity* target = static_cast<Entity*>(intersection->getUserPointer()))
		{
			m_shooter = &shooter;
			m_state = GrapplingHookStatePulling;
			m_point = m_physicsPtr->RayTestGetPoint(Ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE));
		}
	}
	else
	{
		printf("No hit.\n");
	}
}

void SkillGrapplingHook::onRelease()
{
	printf("Released\n");
	m_state = GrapplingHookStateNothing;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };
}

void SkillGrapplingHook::onUpdate(float deltaTime)
{
	if (m_shooter && m_state == GrapplingHookStatePulling)
	{
		printf("Pullin'...\n");
		btRigidBody* shooterBody = m_shooter->getRigidbody();
		btVector3 linearVelocity = shooterBody->getLinearVelocity();
		btVector3 dirToPoint =  (m_point - m_shooter->getPositionBT()).normalize();
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			player->getRigidbody()->setLinearVelocity({ 0, dirToPoint.y() * GRAPPLING_HOOK_SPEED, 0 });
			player->setMoveDirection(dirToPoint);
			player->setMoveSpeed(GRAPPLING_HOOK_SPEED);
		}
	}
}

void SkillGrapplingHook::render(Graphics::Renderer& renderer)
{

}