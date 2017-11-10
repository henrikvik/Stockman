#ifndef WEAPON_H
#define WEAPON_H

#pragma region ClassDesc
		
		/*
			CLASS: Weapon
			AUTHOR:

			DESCRIPTION: This class defines a weapon of the system
		*/

#pragma endregion

#include <d3d11.h>
#include <SimpleMath.h>
#include <btBulletCollisionCommon.h>
#include <vector>

namespace Logic
{
    class Entity;
    class Projectile;
    class ProjectileManager;
    struct ProjectileData;

    typedef std::function<Projectile*(ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter)> SpawnProjectile;

	class Weapon
	{
    public:
        struct WeaponInfo
        {
            int weaponID;
            int projectileCount;
            int spreadH;							// Horizontal spread in degrees	
            int spreadV;							// Vertical spread in degrees
            int attackRate;						    // Attacks per minute (0 = no delay, basically once per frame if no delayTime is set)
            int delayTime;
            float freeze;
        };

	private:
		ProjectileData* m_projectileData;

        WeaponInfo m_wInfo;

		btVector3 calcSpread(float yaw, float pitch);

        SpawnProjectile spawnProjectile;

	public:
		Weapon();
		Weapon(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo);
        virtual ~Weapon();

        void setSpawnFunctions(ProjectileManager &projManager);

		void use(btVector3 position, float yaw, float pitch, Entity& shooter);
        virtual void onUse(std::vector<Projectile*> &projectiles, Entity& shooter) {};

        SpawnProjectile getSpawnProjectileFunc();
		ProjectileData* getProjectileData();
		float getAttackTimer();
        int getDelayTime();
	};
}

#endif