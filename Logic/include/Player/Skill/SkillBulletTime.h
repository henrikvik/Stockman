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
#define BULLET_TIME_SMOOTHNESS_INTERVAL 20
#define BULLET_TIME_SLOW_DOWN_DURATION 1000.f
#define BULLET_TIME_SPEED_UP_DURATION 1000.f

namespace Logic
{
	class SkillBulletTime : public Skill
	{
	private:
		ProjectileData m_projectileData;
		ProjectileManager* m_projectileManager;

		//Projectile* m_travelProjectile;
		Projectile* m_sensor;

		std::vector<float> slowDownIntervals;
		std::vector<float> speedUpIntervals;

	public:
		SkillBulletTime(ProjectileManager* projectileManager, ProjectileData pData);

		void onUse(btVector3 forward, Entity& shooter);
		void onRelease();

		void onUpdate(float deltaTime);
		void render(Graphics::Renderer& renderer);
	};
}
#endif