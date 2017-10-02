#include "Player/Skill/SkillGrapplingHook.h"

using namespace Logic;

SkillGrapplingHook::SkillGrapplingHook(Physics* physics)
: Skill(GRAPPLING_HOOK_CD)
{
	m_physicsPtr = physics;
}

SkillGrapplingHook::~SkillGrapplingHook()
{
	m_physicsPtr = nullptr;
}

void SkillGrapplingHook::onUse(btVector3 forward, Entity& shooter)
{
	const btRigidBody* intersection = m_physicsPtr->RayTestOnRigidBodies(Ray(shooter.getPositionBT(), forward, 500.f));
	if (intersection)
	{
		if (intersection == shooter.getRigidbody())
		{
			printf("Hit! - This enttiy\n");
		}
		else
		{
			printf("Hit! - Something else\n");
			if (Entity* target = static_cast<Entity*>(intersection->getUserPointer()))
			{
				shooter.getRigidbody()->setLinearVelocity(forward * 50);
			}
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
}

void SkillGrapplingHook::onUpdate(float deltaTime)
{

}

void SkillGrapplingHook::render(Graphics::Renderer& renderer)
{

}