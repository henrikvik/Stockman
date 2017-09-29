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

#define GRAPPLING_HOOK_CD 1000.f

namespace Logic
{
	class SkillGrapplingHook : public Skill
	{
	private:
		ProjectileData m_projectileData;
		ProjectileManager* m_projectileManager;

	public:
		SkillGrapplingHook(ProjectileManager* projectileManager, ProjectileData projectileData);
		~SkillGrapplingHook();

		void onUse(btVector3 forward, Entity& shooter);
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif