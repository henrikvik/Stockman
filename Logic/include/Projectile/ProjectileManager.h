#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "ProjectileStruct.h"
#include <vector>

namespace Logic
{
	class ProjectileManager
	{
	public:
		ProjectileManager();
		ProjectileManager(const ProjectileManager& other) = delete;
		ProjectileManager* operator=(const ProjectileManager& other) = delete;
		~ProjectileManager();

		void clear();
		void addProjectile(ProjectileData& pData);
		void removeProjectile();

		std::vector<Projectile*>* getProjectiles();

	private:
		std::vector<Projectile*> m_projectiles;
	};
}


#endif // !PROJECTILEMANAGER_H
