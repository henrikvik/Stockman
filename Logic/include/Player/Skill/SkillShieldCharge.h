#ifndef SKILLSHIELDCHARGE_H
#define SKILLSHIELDCHARGE_H

#pragma region ClassDesc
		/*
			CLASS: SkillShieldCharge
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Projectile\ProjectileManager.h>
#include <Projectile\ProjectileStruct.h>
#include <Player\Skill\Skill.h>

#define SHIELD_CHARGE_CD		1500.f
#define SHIELD_CHARGE_DURATION	300.f

namespace Logic
{
	class SkillShieldCharge : public Skill
	{
	private:
		ProjectileData m_projectileData;
		ProjectileManager* m_projectileManager;
	public:
		SkillShieldCharge(ProjectileManager* projectileManager, ProjectileData projectileData);
		~SkillShieldCharge();

		void onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif