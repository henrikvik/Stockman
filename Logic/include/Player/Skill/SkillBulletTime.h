#ifndef SKILLBULLETTIME_H
#define SKILLBULLETTIME_H
#pragma region ClassDesc
		/*
			CLASS: SkillBulletTime
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Player\Skill\Skill.h>
#include <Projectile\ProjectileManager.h>

#define BULLET_TIME_CD 2500.f
#define BULLET_TIME_DURATION 5000.f

namespace Logic
{
	class SkillBulletTime : public Skill
	{
	private:
		ProjectileData m_projectileData;
		ProjectileManager* m_projectileManager;
	public:
		SkillBulletTime(ProjectileManager* projectileManager, ProjectileData pData);

		void onUse(btVector3 forward, Entity& shooter);
		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif