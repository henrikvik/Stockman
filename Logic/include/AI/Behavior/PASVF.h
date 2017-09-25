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
			inline DirectX::SimpleMath::Vector2 getNormal(DirectX::SimpleMath::Vector2 const &edge) const; // remove?
			inline DirectX::SimpleMath::Vector3 getLine(DirectX::SimpleMath::Vector3 const &p1, DirectX::SimpleMath::Vector3 const &p2) const;
		public:
			enum VERTEX_ORDER { CLOCKWISE, COUNTER_CLOCKWISE };

			struct Triangle {
				DirectX::SimpleMath::Vector3 normal; // normal will be loaded in generation
				DirectX::SimpleMath::Vector3 vertices[3]; // CW
			};

			PASVF();
			virtual ~PASVF();

			void generateNavMesh(NavigationMesh &nav, std::vector<Triangle> terrain, std::vector<std::vector<Triangle>> objects) const;
		private:
			inline DirectX::SimpleMath::Vector3 getNormal(Triangle const &triangle, VERTEX_ORDER vertexOrder = CLOCKWISE) const;
	};
}

#endif