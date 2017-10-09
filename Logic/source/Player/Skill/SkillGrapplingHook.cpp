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
			Ray ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE);
			m_point = m_physicsPtr->RayTestGetPoint(ray);

			// Drawing the ray
			renderDebug.points->clear();
			renderDebug.color = DirectX::SimpleMath::Color(1, 1, 1);
			renderDebug.points->push_back(DirectX::SimpleMath::Vector3(ray.getStart()));
			renderDebug.points->push_back(DirectX::SimpleMath::Vector3(m_point));

		}
	}
}

// On button release
void SkillGrapplingHook::onRelease()
{
	// If unsuccesful hook, don't put full cooldown
	if (!m_shooter)
	{
		setCooldown(GRAPPLING_HOOK_CD);
		setCanUse(true);
	}

	// Set to defaults
	m_state = GrapplingHookStateNothing;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };
	renderDebug.color = DirectX::SimpleMath::Color( 1, 0, 0 );
}

// Moving the entity the grappling hook is active to the targeted point
void SkillGrapplingHook::onUpdate(float deltaTime)
{
	if (m_shooter && m_state == GrapplingHookStatePulling)
	{
		// Getting variables from entity
		btRigidBody* shooterBody = m_shooter->getRigidBody();
		btVector3 linearVelocity = shooterBody->getLinearVelocity();
		btVector3 dirToPoint = (m_point - m_shooter->getPositionBT()).normalize();
		
		// Setting player movement specific grappling hook variables
		if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			// Checks if the player does a 90 degree turn around mid-air
			if (dirToPoint.dot(player->getMoveDirection()) > 0)
			{
				// Resets the vertical velocity to make a "stopping effect"
				player->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
			}
			else
			{
				// If the player targets hooks to something behind himself, reset the current speed to the ground-speed
				player->setMoveSpeed(PLAYER_MOVEMENT_MAX_SPEED);
			}

			// Sets the current movedirection to avoid breaking strafing
			player->setMoveDirection(btVector3(dirToPoint.x(), 0, dirToPoint.z()));

			// Easing to reach the targeted vertical speed
			shooterBody->setLinearVelocity({ 0, linearVelocity.y() + (((dirToPoint.y()) * GRAPPLING_HOOK_MAX_SPEED_Y) - linearVelocity.y()) * GRAPPLING_HOOK_POWER * deltaTime, 0 });

			// Easing to reach the maximum vertical speed
			player->setMoveSpeed(player->getMoveSpeed() + ((GRAPPLING_HOOK_MAX_SPEED_XZ - player->getMoveSpeed()) * GRAPPLING_HOOK_POWER * deltaTime));
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

GrapplingHookState SkillGrapplingHook::getState() const
{
	return m_state;
}
