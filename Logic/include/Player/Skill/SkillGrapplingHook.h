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

#define GRAPPLING_HOOK_CD 500.f

namespace Logic
{
	enum GrapplingHookState
	{
		GrapplingHookStateNothing,
		GrapplingHookStatePulling
	};

	class SkillGrapplingHook : public Skill
	{
	private:
		Physics* m_physicsPtr;
		GrapplingHookState m_state;

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