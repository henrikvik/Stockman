#include "Player/Skill/SkillGrapplingHook.h"
#include <Player\Player.h>
#include <Physics\Physics.h>

using namespace Logic;

#define GRAPPLING_HOOK_CD			    2250.f		// Cooldown in ms
#define GRAPPLING_HOOK_RANGE		    100.f		// Range in bulletphysics units (probably meters)
#define GRAPPLING_HOOK_RANGE_MIN        8.f         // Min range
#define GRAPPLING_HOOK_POWER_XZ		    0.0011f	    // The amount of power to reach the max speed (xz)
#define GRAPPLING_HOOK_POWER_Y		    0.12f	    // y-velocity power
#define GRAPPLING_HOOK_MAX_SPEED_XZ	    0.0615f		// The max speed in x & z
#define GRAPPLING_HOOK_MAX_SPEED_Y	    0.5f		// The max speed in y
#define GRAPPLING_HOOK_DIR_Y_RANGE      0.8f        // default range is -1 to 1, 0.8 makes it -0.8 to 0.8
#define GRAPPLING_HOOK_DIR_Y_MULTIPLIER 0.4f        // the amount the direction affects pull speed (only Y)
#define GRAPPLING_HOOK_NON_HOOK_ANGLE   0.05f       // Can't hook if the angle between up vector and forward is less than this

SkillGrapplingHook::SkillGrapplingHook(Physics* physics)
: Skill(GRAPPLING_HOOK_CD)
{
	// Default variables
	m_physicsPtr = physics;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };
    m_goingUp = true;
    m_maxVelY = 0.f;

	// Debug draw ray
	renderInfo.points = newd std::vector<DirectX::SimpleMath::Vector3>;
	renderInfo.color = DirectX::SimpleMath::Color(1, 1, 1);
	renderInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	renderInfo.useDepth = true;
}

SkillGrapplingHook::~SkillGrapplingHook()
{
	// These are just borrowed
	m_physicsPtr = nullptr;
	m_shooter = nullptr;

	// Debug
	delete renderInfo.points;
}

// When the grappling hook is used, send out a ray to the targeted surface and save variables
bool SkillGrapplingHook::onUse(btVector3 forward, Entity& shooter)
{
    if (m_state == GrapplingHookStatePulling)
        return false;

    if (abs(forward.dot({ 0.f, 1.f, 0.f })) > GRAPPLING_HOOK_NON_HOOK_ANGLE)
    {
        // Saving ray to intersection surface
        Ray ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE);
        btVector3 hookPoint = m_physicsPtr->RayTestGetPoint(ray);
        
        if (abs((hookPoint - shooter.getPositionBT()).length()) > GRAPPLING_HOOK_RANGE_MIN &&
            !(dynamic_cast<Player*>(&shooter)->getCharController()->onGround() && hookPoint.y() < shooter.getPositionBT().y()))
        {
            // Ray testing to see if we're hitting a rigidbody
            const btRigidBody* intersection = m_physicsPtr->RayTestOnRigidBodies(Ray(shooter.getPositionBT(), forward, GRAPPLING_HOOK_RANGE));
            if (intersection)
            {
                if (PhysicsObject* target = static_cast<PhysicsObject*>(intersection->getUserPointer()))
                {
                    // Saving the shooter's as an entity
                    m_shooter = &shooter;

                    // The entity is now pulling the grappling hook
                    m_state = GrapplingHookStatePulling;

                    m_point = hookPoint;

                    if (m_point.y() < shooter.getPositionBT().y())
                        m_goingUp = false;
                    else
                        m_goingUp = true;

                    m_dirToPoint = (m_point - shooter.getPositionBT()).normalize();

                    m_maxVelY = m_dirToPoint.y() * GRAPPLING_HOOK_MAX_SPEED_Y;

                    // Drawing the ray
                    renderInfo.points->clear();
                    renderInfo.color = DirectX::SimpleMath::Color(1, 1, 1);
                    renderInfo.points->push_back(DirectX::SimpleMath::Vector3(ray.getStart()));
                    renderInfo.points->push_back(DirectX::SimpleMath::Vector3(m_point));

                    // grappling sound
                    shooter.getSoundSource()->playSFX(Sound::SFX::SKILL_GRAPPLING);

                    return true;
                }
            }
        }
    }
    return false;
}

// On button release
void SkillGrapplingHook::onRelease()
{
    // Specific release stuff
    setCooldown(GRAPPLING_HOOK_CD);
    setCanUse(false);

	// If unsuccesful hook, don't put full cooldown
/*	if (!m_shooter)
	{
		setCooldown(GRAPPLING_HOOK_CD);
		setCanUse(true);
	}
	else */ if (Player* player = dynamic_cast<Player*>(m_shooter))
	{
		float yVel = player->getCharController()->getLinearVelocity().y();
        if (m_dirToPoint.y() > 0.f)
            player->getCharController()->jump({ 0.f, yVel, 0.f });
	}

    
	// Set to defaults
	m_state = GrapplingHookStateNothing;
	m_shooter = nullptr;
	m_point = { 0, 0, 0 };
    m_dirToPoint = { 0, 0, 0 };
    m_maxVelY = 0.f;
    renderInfo.points->clear();
    renderInfo.color = DirectX::SimpleMath::Color( 1, 0, 0 );
}

// Moving the entity the grappling hook is active to the targeted point
void SkillGrapplingHook::onUpdate(float deltaTime)
{
	if (m_shooter && m_state == GrapplingHookStatePulling)
	{
        // Check if min range
        if ((m_point - m_shooter->getPositionBT()).length() < GRAPPLING_HOOK_RANGE_MIN)
        {
            release();
        }
		// Setting player movement specific grappling hook variables
		else if (Player* player = dynamic_cast<Player*>(m_shooter))
		{
			btGhostObject* ghostObject = player->getGhostObject();
			btVector3 linearVelocity = player->getCharController()->getLinearVelocity();

            if(m_goingUp)
                m_dirToPoint = (m_point - player->getPositionBT()).normalize();
            else if (player->getCharController()->onGround()) // if going down and hitting ground after holding a hook - release
            {
                release();
                return;
            }

			// Sets the current movedirection to avoid breaking strafing
			player->setMoveDirection(btVector3(m_dirToPoint.x(), 0, m_dirToPoint.z()));

			// Easing to reach the targeted vertical speed
			if (m_goingUp && (m_point.y() > player->getPositionBT().y()))
                player->getCharController()->jump({ 0.f, m_maxVelY * (GRAPPLING_HOOK_DIR_Y_RANGE - m_dirToPoint.y() * GRAPPLING_HOOK_DIR_Y_MULTIPLIER) * GRAPPLING_HOOK_POWER_Y, 0.f });
			else
                m_goingUp = false;

			// Easing to reach the maximum vertical speed
			player->setMoveSpeed(player->getMoveSpeed() + ((GRAPPLING_HOOK_MAX_SPEED_XZ - player->getMoveSpeed()) * GRAPPLING_HOOK_POWER_XZ * deltaTime));
		}
		else
		{
			// Getting variables from entity
			btRigidBody* shooterBody = m_shooter->getRigidBody();
			btVector3 linearVelocity = shooterBody->getLinearVelocity();
			btVector3 dirToPoint = (m_point - m_shooter->getPositionBT()).normalize();
		}

		// Setting entity movement specific varialbes
		//
	}
}

void SkillGrapplingHook::onReset()
{
    m_state = GrapplingHookStateNothing;
    m_shooter = nullptr;
    m_point = { 0, 0, 0 };
    m_maxVelY = 0.f;
    renderInfo.points->clear();
    renderInfo.color = DirectX::SimpleMath::Color(1, 0, 0);
}

void SkillGrapplingHook::onUpgradeAdd(int stacks, Upgrade const & upgrade)
{
}

void SkillGrapplingHook::render() const
{
	// Drawing a ray of the grappling hook for debugging purposes
    QueueRender(renderInfo);
}

GrapplingHookState SkillGrapplingHook::getState() const
{
	return m_state;
}

