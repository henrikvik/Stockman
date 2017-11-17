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

#define BULLET_TIME_DURATION 7000.f

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
        virtual void onReset();

		void onUpdate(float deltaTime);
		virtual void render() const override;

	};
}
#endif