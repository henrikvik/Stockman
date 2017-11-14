#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

namespace Logic
{
    enum DIRECTION_FLAG {
        NOTHING = 0,
        DIR_LEFT = 0x1,
        DIR_RIGHT = 0x2,
        DIR_FORWARD = 0x4,
        DIR_BACKWARD = 0x8,
    };

    class Physics;
    class PlayerMovement
    {
    public:

        enum PlayerState
        {
            STANDING,
            CROUCHING,
            IN_AIR
        };

        void warpToOrigin();
        void activeNoClip();
        void deactivateNoClip();
        void noClipMoveUp();
        void noClipMoveDown();
        void moveMouse(int x, int y);
        void wantToJump();
        void doesNotWantToJump();
        void moveDirection(DIRECTION_FLAG direction);

        void update(float deltaTime);

        btKinematicCharacterController* getCharController();
        btGhostObject* getGhostObject();
        btVector3 getPositionBT() const;

    private:

        // Strafing God
        void move(float deltaTime);
        void airMove(float deltaTime);
        void accelerate(float deltaTime, float acceleration);
        void applyFriction(float deltaTime, float friction);
        void applyAirFriction(float deltaTime, float friction);

        float m_dt;
        btKinematicCharacterController* m_charController;
        Physics* m_physicsPointer;

        // Movement variables
        PlayerState m_playerState;
        btVector3 m_forward;
        float m_moveMaxSpeed;
        btVector3 m_moveDir; // only 2 dimensional movement direction (x, z)
        float m_moveSpeed;
        float m_acceleration;
        float m_deacceleration;
        float m_airAcceleration;
        float m_jumpSpeed;
        bool m_wishJump;
        btVector3 m_wishDir;
        float m_wishDirForward;
        float m_wishDirRight;

        // Mouse
        float m_mouseSens;
        float m_camYaw;
        float m_camPitch;

        // Debugging
        bool m_godMode;
        bool m_noclip;
    };
}

#endif // !PLAYERMOVEMENT_H