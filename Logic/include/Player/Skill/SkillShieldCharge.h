#ifndef SKILLSHIELDCHARGE_H
#define SKILLSHIELDCHARGE_H

#pragma region ClassDesc
		/*
			CLASS: SkillShieldCharge
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Player\Skill\Skill.h>
#include <btBulletCollisionCommon.h>

namespace Logic
{
    class Entity;
    class ProjectileManager;
    class Projectile;
    struct ProjectileData;

	class SkillShieldCharge : public Skill
	{
	private:
		bool m_slowdown;
		float m_time;
		btVector3 m_forw;
		Entity *m_shooter;
		float m_chargePower;
        float m_fovM;               // FOV multiplier for display-effect
        float m_oldSpeed;
        ProjectileData* m_pData;
        Projectile* m_projectile;

        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)> SpawnProjectile;

        void setSpawnFunctions(ProjectileManager &projManager);
	public:
		SkillShieldCharge(ProjectileManager* projectileManager, ProjectileData& pData);
		~SkillShieldCharge();

        bool onUse(btVector3 forward, Entity& shooter);
		void onRelease();
		void onUpdate(float deltaTime);
        void onUpgradeAdd(int stacks, Upgrade const & upgrade);
        virtual void render() const override;

	};
}
#endif
