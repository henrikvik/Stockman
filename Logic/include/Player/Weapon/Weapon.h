#ifndef WEAPON_H
#define WEAPON_H

#pragma region ClassDesc
		
		/*
			CLASS: Weapon
			AUTHOR:

			DESCRIPTION: This class defines a weapon of the system
		*/

#pragma endregion

#include <Graphics\include\RenderQueue.h>
#include <btBulletCollisionCommon.h>
#include <vector>

#include <Projectile\ProjectileStruct.h>

namespace Logic
{
    class Entity;
    class Projectile;
    class ProjectileManager;

    typedef std::function<Projectile*(ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter, btVector3 modelOffset)> SpawnProjectile;

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
            btVector3 projectileOffset;
            float swapTimer;
        };

	private:
		ProjectileData m_projectileData;

        WeaponInfo m_wInfo;

		btVector3 calcSpread(float yaw, float pitch);

        SpawnProjectile spawnProjectile;

        
	public:
		Weapon();
		Weapon(ProjectileManager* projectileManager, ProjectileData& projectileData, WeaponInfo wInfo);
        virtual ~Weapon();

        void setSpawnFunctions(ProjectileManager &projManager);

		void use(btVector3 position, float yaw, float pitch, Entity& shooter);
        virtual void onUse(std::vector<Projectile*> &projectiles, Entity& shooter) {};
        virtual bool useEnhanced(bool shouldUse) { return true; };

        SpawnProjectile getSpawnProjectileFunc();
		ProjectileData& getProjectileData();
		float getAttackTimer(float modifier);
        int getDelayTime();
        float getSwapTimer();
    };
}

#endif