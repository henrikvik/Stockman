#ifndef SKILLGRAPPLINGHOOK_H
#define SKILLGRAPPLINGHOOK_H

#pragma region ClassDesc
		/*
		CLASS: SkillGrapplingHook
		AUTHOR: Simon Fredholm

		DESCRIPTION:
		Handles all the actions for the grappling hook
		Sends out a ray and gets the targeted area
		Sends the entity that shot with a constant velocity (added on top of the movement from the entity) towards the point
		*/
#pragma endregion

#include <Player\Skill\Skill.h>
#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>

#define GRAPPLING_HOOK_CD			50.f		// Cooldown in ms
#define GRAPPLING_HOOK_RANGE		500.f		// Range in bulletphysics units (probably meters)
#define GRAPPLING_HOOK_MAX_SPEED	0.05f		// The movement speed for the player in specific when used
#define GRAPPLING_HOOK_POWER		0.001f		// The amount of power to reach the max speed

namespace Logic
{
	enum GrapplingHookState
	{
		GrapplingHookStateNothing,
		GrapplingHookStatePulling
	};

	class Player;
	class SkillGrapplingHook : public Skill
	{
	public:
		SkillGrapplingHook(Physics* physics);
		~SkillGrapplingHook();

		void onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);

		GrapplingHookState getState() const;
	private:
		Physics*						m_physicsPtr;	//< Just a pointer to the physics to be able to raycast
		GrapplingHookState				m_state;		//< Current state, if the grappling hook is currently pulling or not
		Entity*							m_shooter;		//< Saved entity after each onUse() call, later, pushes this entity
		btVector3						m_point;		//< Saved point of intersection of the raytest, will push entity towards this point
		Graphics::RenderDebugInfo		renderDebug;	//< Debug drawing the ray
	};
}
#endif