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
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Graphics
{
    class Renderer;
    struct RenderDebugInfo;
}

namespace Logic
{
    class Entity;
    class Physics;

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

		bool onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);

		GrapplingHookState getState() const;
	private:
		Physics*						m_physicsPtr;	//< Just a pointer to the physics to be able to raycast
		GrapplingHookState				m_state;		//< Current state, if the grappling hook is currently pulling or not
		Entity*							m_shooter;		//< Saved entity after each onUse() call, later, pushes this entity
		btVector3						m_point;		//< Saved point of intersection of the raytest, will push entity towards this point
        bool                            m_goingUp;      //< Direction of the grappling hook
        btVector3                       m_dirToPoint;
		Graphics::RenderDebugInfo*		renderDebug;
        void onUpgrade(Upgrade const & upgrade);
        //< Debug drawing the ray
	};
}
#endif