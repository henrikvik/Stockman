#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <Physics\Physics.h>
#include <Projectile\Projectile.h>
#include <vector>

namespace Logic
{
	class ProjectileManager
	{
	public:
		ProjectileManager(Physics* physPtr);
		ProjectileManager(const ProjectileManager& other) = delete;
		ProjectileManager* operator=(const ProjectileManager& other) = delete;
		~ProjectileManager();

        void init();
		void clear();
		Projectile* addProjectile(ProjectileData& pData, btVector3 position, btVector3 forward, Entity& shooter);
		void removeProjectile(Projectile* p);
		void removeProjectile(Projectile* p, int index);

		void update(float deltaTime);
		void render(Graphics::Renderer &renderer);

		std::vector<Projectile*>& getProjectiles();

	private:
		std::vector<Projectile*> m_projectilesActive;
        std::vector<Projectile*> m_projectilesIdle;
		Physics* m_physPtr;
	};
}


#endif // !PROJECTILEMANAGER_H
