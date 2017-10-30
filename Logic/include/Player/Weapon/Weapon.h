#ifndef WEAPON_H
#define WEAPON_H

#pragma region ClassDesc
		
		/*
			CLASS: Weapon
			AUTHOR:

			DESCRIPTION: This class defines a weapon of the system
		*/

#pragma endregion

#include <Entity\Object.h>
#include <btBulletCollisionCommon.h>

namespace Logic
{
    class Entity;
    class Projectile;
    class ProjectileManager;
    struct ProjectileData;

	class Weapon : public Object
	{
    public:
        struct WeaponInfo
        {
            int weaponID;
            int projectileCount;
            int spreadH;							// Horizontal spread in degrees	
            int spreadV;							// Vertical spread in degrees
            float attackRate;						// Attacks per minute
            float freeze;
        };

	private:
		DirectX::SimpleMath::Matrix rot, trans, scale;
		ProjectileData* m_projectileData;

        WeaponInfo m_wInfo;
	//	Animation m_animation;

		btVector3 calcSpread(float yaw, float pitch);

        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)> SpawnProjectile;

	public:
		Weapon();
		Weapon(Graphics::ModelID modelID, ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo);
        ~Weapon();

        void setSpawnFunctions(ProjectileManager &projManager);

		void use(btVector3 position, float yaw, float pitch, Entity& shooter);
		void setWeaponModelFrontOfPlayer(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

		ProjectileData* getProjectileData();
		float getAttackTimer();
	};
}

#endif