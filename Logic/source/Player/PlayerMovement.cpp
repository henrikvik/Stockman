#include <Player\PlayerMovement.h>
#include <Singeltons\Profiler.h>
//#include <Physics\Physics.h>

using namespace Logic;

#define TO_RADIANS(degrees) degrees * (3.14 / 180)

#define PLAYER_GRAVITY					9.82f * 2.f * 0.0000015f
#define PLAYER_SIZE_RADIUS				0.5f
#define PLAYER_SIZE_HEIGHT				2.f
#define PLAYER_STARTING_HP				3
#define PLAYER_MOUSE_SENSETIVITY		0.01f
#define PLAYER_MOVEMENT_MAX_SPEED		0.015f
#define PLAYER_MOVEMENT_ACCELERATION	0.0002f
#define PLAYER_MOVEMENT_AIRACCELERATION	0.005f
#define PLAYER_MOVEMENT_AIRSTRAFE_SPEED 0.004f
#define PLAYER_SPEED_LIMIT				0.04f
#define PLAYER_STRAFE_ANGLE				0.95f
#define PLAYER_FRICTION					20.f
#define PLAYER_AIR_FRICTION				1.f
#define PLAYER_JUMP_SPEED				0.008f

void PlayerMovement::warpToOrigin()
{
    m_charController->warp({ 0.f, 0.f, 0.f });
    m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
    m_moveDir = { 0.f, 0.f, 0.f };
    m_moveSpeed = 0.f;
}

void PlayerMovement::activeNoClip()
{
    m_charController->setGravity({ 0.f, 0.f, 0.f }); // remove gravity
    m_noclip = true;
    printf("free move activated\n");
}

void PlayerMovement::deactivateNoClip()
{
    m_charController->setGravity({ 0.f, -PLAYER_GRAVITY, 0.f });
    m_moveDir.setZero();
    m_moveSpeed = 0.f;
    m_noclip = false;
    printf("free move deactivated\n");
}

void PlayerMovement::noClipMoveUp()
{
    if (m_noclip)
    {
        m_wishDir.setY(-0.1f * m_dt);
        m_charController->warp(getPositionBT() + (m_wishDir * m_moveMaxSpeed * 2.f * m_dt));
    }
}

void PlayerMovement::noClipMoveDown()
{
    if (m_noclip)
    {
        m_wishDir.setY(0.1f * m_dt);
        m_charController->warp(getPositionBT() + (m_wishDir * m_moveMaxSpeed * 2.f * m_dt));
    }
}

void PlayerMovement::moveMouse(int x, int y)
{
    m_camYaw += m_mouseSens * (x * m_dt);
    m_camPitch -= m_mouseSens * (y * m_dt);

    // Pitch lock and yaw correction
    if (m_camPitch > 89)
        m_camPitch = 89;
    if (m_camPitch < -89)
        m_camPitch = -89;
    if (m_camYaw < 0.f)
        m_camYaw += 360.f;
    if (m_camYaw > 360.f)
        m_camYaw -= 360.f;

    // Create forward
    m_forward.setX(cos(TO_RADIANS(m_camPitch)) * cos(TO_RADIANS(m_camYaw)));
    m_forward.setY(sin(TO_RADIANS(m_camPitch)));
    m_forward.setZ(cos(TO_RADIANS(m_camPitch)) * sin(TO_RADIANS(m_camYaw)));

    m_forward.normalize();
}

void PlayerMovement::wantToJump()
{
    if (!m_wishJump && m_playerState != PlayerState::IN_AIR)
        m_wishJump = true;
}

void PlayerMovement::doesNotWantToJump()
{
    m_wishJump = false;
}

void PlayerMovement::moveDirection(DIRECTION_FLAG direction)
{
    // Reset from last time
    m_wishDir.setZero();
    m_wishDirForward = 0.f;
    m_wishDirRight = 0.f;

    if (direction & DIR_LEFT)
    {
        btVector3 dir = btVector3(m_forward.x(), 0, m_forward.z()).cross({ 0, 1, 0 }).normalize();
        m_wishDir += -dir;
        m_wishDirRight += -1.f;
    }

    if (direction & DIR_RIGHT)
    {
        btVector3 dir = btVector3(m_forward.x(), 0, m_forward.z()).cross({ 0, 1, 0 }).normalize();
        m_wishDir += dir;
        m_wishDirRight += 1.f;
    }

    if (direction & DIR_FORWARD)
    {
        btVector3 dir = btVector3(m_forward.x(), 0, m_forward.z()).normalize();
        m_wishDir += dir;
        m_wishDirForward += 1.f;
    }

    if (direction & DIR_BACKWARD)
    {
        btVector3 dir = btVector3(m_forward.x(), 0, m_forward.z()).normalize();
        m_wishDir += -dir;
        m_wishDirForward += -1.f;
    }

    if (!m_wishDir.isZero())
        m_wishDir = m_wishDir.normalize();
}

void PlayerMovement::update(float deltaTime)
{
    // Saving the deltaTime for certain movement functions
    m_dt = deltaTime;    
   
    if (!m_noclip)
    {
        if (m_playerState == PlayerState::STANDING)
            // Move
            move(deltaTime);
        else if (m_playerState == PlayerState::IN_AIR)
            // Move in air
            airMove(deltaTime);
    }

    if (m_charController->onGround())
    {
        m_playerState = PlayerState::STANDING;
        m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
    }
    else
        m_playerState = PlayerState::IN_AIR;
}

void PlayerMovement::move(float deltaTime)
{
    // On ground
    if (!m_wishJump)
    {
        float friction = (m_moveMaxSpeed * 2 - (m_moveMaxSpeed - m_moveSpeed)) * PLAYER_FRICTION; // smooth friction
        applyFriction(deltaTime, friction > 0.1f ? friction : 0.1f);

        // if player wants to move
        if (!m_wishDir.isZero())
        {
            // Reset movement speed if changing direction
            if (m_moveDir.dot(m_wishDir) <= 0.f)
                m_moveSpeed = 0.f;

            // Change move direction
            m_moveDir = m_wishDir;
        }
    }
    // On ground and about to jump
    else
    {
        m_airAcceleration = (PLAYER_SPEED_LIMIT - m_moveSpeed) * PLAYER_MOVEMENT_AIRACCELERATION;

        if (!m_wishDir.isZero() && m_moveDir.dot(m_wishDir) <= 0.f)
            applyAirFriction(deltaTime, PLAYER_AIR_FRICTION * 6.f);		// if trying to move in opposite direction in air apply more friction
        else
            applyAirFriction(deltaTime, PLAYER_AIR_FRICTION);
    }

    // Apply acceleration and move player
    if (m_wishDir.isZero() || m_wishJump)
        accelerate(deltaTime, 0.f);
    else
        accelerate(deltaTime, m_acceleration);

    // Apply jump if player wants to jump
    if (m_wishJump)
    {
        m_charController->jump({ 0.f, PLAYER_JUMP_SPEED, 0.f });
        m_wishJump = false;
    }
}

void PlayerMovement::airMove(float deltaTime)
{
    applyAirFriction(deltaTime, (m_moveMaxSpeed - (m_moveMaxSpeed - m_moveSpeed)) * PLAYER_AIR_FRICTION); // smooth friction

    accelerate(deltaTime, m_airAcceleration);

    m_airAcceleration = 0.f;
}

void PlayerMovement::accelerate(float deltaTime, float acceleration)
{
    if (deltaTime * 0.001f > (1.f / 60.f))
        deltaTime = (1.f / 60.f) * 1000.f;

    m_moveSpeed += acceleration * deltaTime;

    if (m_playerState != PlayerState::IN_AIR && !m_wishJump && m_moveSpeed > m_moveMaxSpeed)
        m_moveSpeed = m_moveMaxSpeed;

    // Apply moveDir and moveSpeed to player
    if (m_playerState != PlayerState::IN_AIR)
        m_charController->setVelocityForTimeInterval(m_moveDir * m_moveSpeed, deltaTime);
    else
        m_charController->setVelocityForTimeInterval(((m_moveDir + btVector3(0.f, m_charController->getLinearVelocity().y(), 0.f)) * m_moveSpeed) + (m_wishDir * PLAYER_MOVEMENT_AIRSTRAFE_SPEED), deltaTime);

    // Step player
    // TODO What did this do? It moves the player, but this class isn't in use
    //m_charController->preStep(m_physicsPointer);
    //m_charController->playerStep(m_physicsPointer, deltaTime);
}

void PlayerMovement::applyFriction(float deltaTime, float friction)
{
    float toDrop = m_deacceleration * deltaTime * friction;

    m_moveSpeed -= toDrop;
    if (m_moveSpeed < 0)
        m_moveSpeed = 0;
}

void PlayerMovement::applyAirFriction(float deltaTime, float friction)
{
    btVector3 rightMoveDir = m_moveDir.cross(btVector3(0.f, 1.f, 0.f));
    btVector3 forward = btVector3(m_forward.x(), 0.f, m_forward.z()).safeNormalize(); // forward vector (look direction)
    float lookMoveAngle = m_moveDir.dot(forward);
    float lookMovedirection = rightMoveDir.dot(forward);

    // if looking backwards compared to move direction
    if (lookMoveAngle < 0.f)
    {
        lookMoveAngle *= -1.f;
        forward *= -1.f;
    }

    if (lookMovedirection < 0.f && m_wishDirRight < 0.f)
    {
        if (lookMoveAngle > PLAYER_STRAFE_ANGLE)
            m_moveDir = (m_moveDir + forward) * 0.5f;
        else
        {
            m_airAcceleration = 0.f;
            applyFriction(deltaTime, friction);
        }
    }
    else if (lookMovedirection > 0.f && m_wishDirRight > 0.f)
    {
        if (lookMoveAngle > PLAYER_STRAFE_ANGLE)
            m_moveDir = (m_moveDir + forward) * 0.5f;
        else
        {
            m_airAcceleration = 0.f;
            applyFriction(deltaTime, friction);
        }
    }
    else
    {
        m_airAcceleration = 0.f;
        applyFriction(deltaTime, friction);
    }
}

btKinematicCharacterController * PlayerMovement::getCharController() { return m_charController; }
btGhostObject * PlayerMovement::getGhostObject() { return m_charController->getGhostObject(); }
btVector3 PlayerMovement::getPositionBT() const { return m_charController->getGhostObject()->getWorldTransform().getOrigin(); }