#ifndef GRAPPLINGPOINT_H
#define GRAPPLINGPOINT_H


#pragma region ClassDesc
	/*
		CLASS: Grappling Point
		AUTHOR: Simon Fredholm

		DESCRIPTION: Very simple class for the projectile from the grappling hook to hit
					Also checks with a ray-trace from the player's direction if the player is 
					currently looking at the grappling-point's area, if so, the grapplingpoint will make it
					easier to hit & also make a little particle-effect to indicate to the player to press!

		IT DOESN'T WORK CORRECTLY:
			- Don't forget to init() each created grappling-point.
			- The rigidbody body in the constructor should be a sphere
			- If the player class is deleted somewhere in the game, and remade, the init() function needs to be called again (this is a problem for more than just this class tho)
	*/
#pragma endregion

#include <Entity\Entity.h>
#include <Projectile\Projectile.h>

#define GP_RAY_TRACE_FRAME		5					// How often we should raytrace the players hitscan
#define GP_RAY_TRACE_DISTANCE	100					// How far the raytrace should go before we exit the check

#define GP_INVIS_MOD			3.f					// The scale-modifier on the invisible hitbox 
#define GP_INVIS_ROT			{ 0.f, 0.f, 0.f }	// The rotation on the invisible hitbox
#define GP_INVIS_MASS			0.f					// Should be kept at zero
#define GP_INVIS_DEFAULT_RADIUS 1.f					// If the grapplingpoint is not created as a sphere, should not be used if created properly

#define GP_POWER				55.f				// The amount of power of the impulse when hit
#define GP_ACC					1.05f				// Amount of accerliraction on each hit

namespace Logic
{
	class Player;
	class Physics;
	class GrapplingPoint : public Entity
	{
	public:
		GrapplingPoint(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::SPHERE);
		~GrapplingPoint();

		void init(Physics* physics, Player* player);
		void updateSpecific(float deltaTime);
		void onCollision(Entity& other);
		
	private:	
		Physics*		m_physicsPtr;		//< We need physics because we need to perform a raytest & create an invisible hitbox
		Player*			m_playerPtr;		//< We need the forward & position from the player to perform a raytest
		
		// Indicator that you're aiming close to a grappling hook
		btRigidBody*	m_invisBox;			//< The invisible box that we want to raytest on
		btVector3		m_halfExtentNormal;	//< The proper scale of the graphical side
		bool			m_correctAim;		//< If the player is aiming directly on the invisible box

	};

}

#endif // !GRAPPLINGPOINT_H
