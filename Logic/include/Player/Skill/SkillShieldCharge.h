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

#define SHIELD_CHARGE_CD 2500.f
#define SHIELD_CHARGE_DURATION	1000.f

namespace Logic
{
	class Player;
	class SkillShieldCharge : public Skill
	{
	private:
		ProjectileData m_projectileData;
		ProjectileManager* m_projectileManager;
		bool m_active;
		float m_time;
		btVector3 m_forw;
		Entity* m_thePlayer;
		float m_chargePower;
	public:
		SkillShieldCharge();
		~SkillShieldCharge();

		void onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif
