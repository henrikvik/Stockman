#ifndef SKILLGRAPPLINGHOOK_H
#define SKILLGRAPPLINGHOOK_H
#pragma region ClassDesc
		/*
			CLASS: SkillGrapplingHook
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Player\Skill\Skill.h>
#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>

#define GRAPPLING_HOOK_CD	 500.f
#define GRAPPLING_HOOK_RANGE 500.f
#define GRAPPLING_HOOK_SPEED 0.06f

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
	private:
		Physics* m_physicsPtr;
		GrapplingHookState m_state;
		Entity* m_shooter;
		btVector3 m_point;
		Graphics::RenderDebugInfo renderDebug;

	public:
		SkillGrapplingHook(Physics* physics);
		~SkillGrapplingHook();

		void onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif