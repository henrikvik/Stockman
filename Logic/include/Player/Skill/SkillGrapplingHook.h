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

namespace Logic
{
    class Entity;
    class Physics;

	class Player;
	class SkillGrapplingHook : public Skill
	{
	public:
		SkillGrapplingHook(Physics* physics);
		~SkillGrapplingHook();

		bool onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
        virtual void onReset();
        virtual void render() const override;
        void onUpgradeAdd(int stacks, Upgrade const & upgrade);

	private:
		Physics*						m_physicsPtr;	//< Just a pointer to the physics to be able to raycast
		Entity*							m_shooter;		//< Saved entity after each onUse() call, later, pushes this entity
		btVector3						m_point;		//< Saved point of intersection of the raytest, will push entity towards this point
        bool                            m_goingUp;      //< Direction of the grappling hook
        btVector3                       m_dirToPoint;
        float                           m_maxVelY;

		DebugRenderInfo                 renderInfo;
        // Inherited via Skill
        //< Debug drawing the ray
	};
}
#endif