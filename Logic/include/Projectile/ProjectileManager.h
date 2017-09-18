#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <Physics\Physics.h>
#include "Projectile.h"
#include "ProjectileStruct.h"
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

		void clear();
		void addProjectile(ProjectileData& pData, btVector3 forward);
		void removeProjectile();

		std::vector<Projectile*>* getProjectiles();

	private:
		std::vector<Projectile*> m_projectiles;
		Physics* m_physPtr;
	};
}


#endif // !PROJECTILEMANAGER_H
