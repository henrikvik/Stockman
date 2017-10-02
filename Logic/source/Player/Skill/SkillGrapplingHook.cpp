#include "Player/Skill/SkillGrapplingHook.h"
#include <Player\Player.h>

using namespace Logic;

SkillGrapplingHook::SkillGrapplingHook(Physics* physics)
: Skill(GRAPPLING_HOOK_CD)
{
	// Default variables
	m_physicsPtr = physics;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };

	// Debug draw ray
	renderDebug.points = new std::vector<DirectX::SimpleMath::Vector3>;
	renderDebug.color = DirectX::SimpleMath::Color(1, 1, 1);
	renderDebug.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	renderDebug.useDepth = true;
}

SkillGrapplingHook::~SkillGrapplingHook()
{
	// These are just borrowed
	m_physicsPtr = nullptr;
	m_shooter = nullptr;

	// Debug
	delete renderDebug.points;
}

// When the grappling hook is used, send out a ray to the targeted surface and save variables
void SkillGrapplingHook::onUse(btVector3 forward, Entity& shooter)
{
	// Ray testing to see if we're hitting a rigidbody
	const btRigidBody* intersection = m_physicsPtr->RayTestOnRigidBodies(Ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE));
	if (intersection)
	{
		if (Entity* target = static_cast<Entity*>(intersection->getUserPointer()))
		{
			// Saving the shooter's as an entity
			m_shooter = &shooter;

			// The entity is now pulling the grappling hook
			m_state = GrapplingHookStatePulling;

			// Saving ray to intersection surface
			m_point = m_physicsPtr->RayTestGetPoint(Ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE));

			// Drawing the ray
			renderDebug.points->clear();
			renderDebug.points->push_back(DirectX::SimpleMath::Vector3(shooter.getPositionBT()));
			renderDebug.points->push_back(DirectX::SimpleMath::Vector3(m_point));

		}
	}
}

// On button release
void SkillGrapplingHook::onRelease()
{
	// Set to defaults
	m_state = GrapplingHookStateNothing;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };
}

// Moving the entity the grappling hook is active to the targeted point
void SkillGrapplingHook::onUpdate(float deltaTime)
{
	if (m_shooter && m_state == GrapplingHookStatePulling)
	{
		// Getting variables from entity
		btRigidBody* shooterBody = m_shooter->getRigidbody();
		btVector3 linearVelocity = shooterBody->getLinearVelocity();
		btVector3 dirToPoint = (m_point - m_shooter->getPositionBT()).normalize();
		
		// Setting player movement specific grappling hook variables
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			player->getRigidbody()->setLinearVelocity({ 0, dirToPoint.y() * GRAPPLING_HOOK_SPEED, 0 });
			player->setMoveDirection(dirToPoint);
			player->setMoveSpeed(GRAPPLING_HOOK_SPEED);
		}

		// Setting entity movement specific varialbes
		//
	}
}

void SkillGrapplingHook::render(Graphics::Renderer& renderer)
{
	// Drawing a ray of the grappling hook for debugging purposes
	renderer.queueRenderDebug(&renderDebug);
}