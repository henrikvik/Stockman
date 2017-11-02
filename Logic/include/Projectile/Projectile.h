#ifndef PROJECTILE_H
#define PROJECTILE_H

#pragma region Comment

/*

    Description: Wrapper for the ProjectileData struct
                    Have different functions to handle how projectile act

*/

#pragma endregion Description of class

#include <Entity/Entity.h>
#include <Projectile/ProjectileStruct.h>
#include <Misc/NonCopyable.h>

namespace Logic
{
    class Player;
	class Projectile : public Entity, public NonCopyable
	{
	public:
		Projectile(btRigidBody* body, btVector3 halfExtent, ProjectileData pData);
		~Projectile();

		void start(btVector3 forward, StatusManager& statusManager);
        void updateSpecific(float deltaTime);
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);

		void upgrade(Upgrade const &upgrade);
		void affect(int stacks, Effect const & effect, float deltaTime);

        // Get & Sets
        void setProjectileData(ProjectileData pData);
		ProjectileData& getProjectileData();
        void setDead(bool dead);
		bool getDead() const;

	private:        
        ProjectileData m_pData; //< Holds information about this projectile
		float m_bulletTimeMod;  //< The amount of slowdown from bulletTime, currently affecting this projectile
		bool m_dead;            //< If this projectile should get removed or not

        // Private functions
        bool collisionWithProjectile(Projectile* proj);
        bool collisionWithPlayer(Player* player);
        bool collisionWithEnemy(Enemy* enemy);
        bool collisionWithTerrain();
        void doCallBack(PhysicsObject& other);
    };
}

#endif // !PROJECTILE_H