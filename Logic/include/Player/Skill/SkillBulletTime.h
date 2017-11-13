#ifndef SKILLBULLETTIME_H
#define SKILLBULLETTIME_H
#pragma region ClassDesc
		/*
			CLASS: SkillBulletTime
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <vector>
#include <Player\Skill\Skill.h>
#include <btBulletCollisionCommon.h>

#define BULLET_TIME_CD 2500.f
#define BULLET_TIME_DURATION 5000.f
#define BULLET_TIME_SMOOTHNESS_INTERVAL 20
#define BULLET_TIME_SLOW_DOWN_DURATION 1000.f
#define BULLET_TIME_SPEED_UP_DURATION 1000.f

namespace Logic
{
    class Entity;
    class ProjectileManager;
    class Projectile;
    struct ProjectileData;

	class SkillBulletTime : public Skill
	{
	private:
		ProjectileData* m_pData;

		//Projectile* m_travelProjectile;
		Projectile* m_sensor;

		std::vector<float> slowDownIntervals;
		std::vector<float> speedUpIntervals;

		int m_stacks;

        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)> SpawnProjectile;

        SpecialEffectRenderInfo renderInfo;
	public:
		SkillBulletTime(ProjectileManager* projectileManager, ProjectileData pData);
        ~SkillBulletTime();

        void setSpawnFunctions(ProjectileManager &projManager);

		bool onUse(btVector3 forward, Entity& shooter);
		void onRelease();

		void onUpdate(float deltaTime);
        void onAffect(Effect const & effect);
		virtual void render() const override;

	};
}
#endif