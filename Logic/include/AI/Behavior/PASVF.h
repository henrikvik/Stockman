#ifndef PASVF_H
#define PASVF_H

#include "NavigationMesh.h"
#include <Entity\Entity.h>

#include <vector>

namespace Logic
{
	// Lukas implementation of Planar Adaptive Space Filling Volumes.
	/*
		Did you ever hear the tragedy of Planar Adaptive Space Filling Volumes? I thought not. It’s not a story the teachers would tell you.
		It’s a AI legend.
		*The rest of the comment is corrupted, you can't read it*
	*/

	class PASVF 
	{
		private:
			inline DirectX::SimpleMath::Vector2 getNormal(DirectX::SimpleMath::Vector2 const &edge) const;
		public:
			PASVF();
			virtual ~PASVF();

			NavigationMesh generateNavMesh(float x, float y, int width, int height, std::vector<Entity*> collidableObjects) const;
	};
}

#endif
