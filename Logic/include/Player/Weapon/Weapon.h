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

namespace Logic
{
    class Entity;
    class Projectile;
    class ProjectileManager;
    struct ProjectileData;

	class Weapon
	{
    public:
        struct WeaponInfo
        {
            int weaponID;
            int projectileCount;
            int spreadH;							// Horizontal spread in degrees	
            int spreadV;							// Vertical spread in degrees
            float attackRate;						// Attacks per minute
            float delayTime;
            float freeze;
        };

	private:
		ProjectileData* m_projectileData;

        WeaponInfo m_wInfo;

		btVector3 calcSpread(float yaw, float pitch);

        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)> SpawnProjectile;

	public:
		Weapon();
		Weapon(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo);
        ~Weapon();

        void setSpawnFunctions(ProjectileManager &projManager);

		void use(btVector3 position, float yaw, float pitch, Entity& shooter);

		ProjectileData* getProjectileData();
		float getAttackTimer();
        float getDelayTime();
	};
}

#endif