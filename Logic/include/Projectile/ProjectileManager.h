#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <btBulletCollisionCommon.h>

namespace Graphics
{
    class Renderer;
}

namespace Logic
{
    class Entity;
    class Projectile;
    class Physics;
    struct ProjectileData;

	class ProjectileManager
	{
	public:
		ProjectileManager(Physics* physPtr);
		ProjectileManager(const ProjectileManager& other) = delete;
		ProjectileManager* operator=(const ProjectileManager& other) = delete;
		~ProjectileManager();

        void init();
		void clear();
        Projectile* addProjectile(ProjectileData& pData, btVector3 position, btVector3 forward, Entity& shooter, btVector3 modelOffset = { 0.f, 0.f, 0.f });
		void removeProjectile(Projectile* p, int index);
        void removeAllProjectiles();

		void update(float deltaTime);
		void render();

		std::vector<Projectile*>& getProjectiles();

	private:
		std::vector<Projectile*> m_projectilesActive;
        std::vector<Projectile*> m_projectilesIdle;
		Physics* m_physPtr;
	};
}


#endif // !PROJECTILEMANAGER_H
