#ifndef PASVF_H
#define PASVF_H

#include "NavigationMesh.h"
#include <Entity\Object.h>

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

		public:
			PASVF();
			virtual ~PASVF();

			NavigationMesh generateNavMesh(int x, int y, int width, int height, std::vector<Object> collidableObjects) const;
	};
}

#endif
