#ifndef PASVF_H
#define PASVF_H

#include "NavigationMesh.h"

#include <d3d11.h>
#include <SimpleMath.h>
#include <Entity\Entity.h>
#include <cmath>

#include <vector>

namespace Logic
{
	//          Nav Mesh Gen v0.1
    // For errors in creating the navigation mesh
    // create a issue on github[1] or contact LW.
    // Based on the NavMesh generation in Tom Clancy's Ghost Recon[2]
    // game series.
    // 
    // [1]: https://github.com/henrikvik/DV1544-Stort-Spel/issues
    // [2]: http://store.steampowered.com/app/15300/Tom_Clancys_Ghost_Recon/

    class Physics;
	class NavigationMeshGeneration 
	{
		public:
			enum VertexOrder { CLOCKWISE, COUNTER_CLOCKWISE };

			struct Triangle {
				DirectX::SimpleMath::Vector3 normal; // normal will be loaded in generation
				DirectX::SimpleMath::Vector3 vertices[3]; // CW
			};

            struct StaticObject {
                std::vector<Triangle> tris;
            };

			NavigationMeshGeneration();
			virtual ~NavigationMeshGeneration();

            void registerGenerationCommand(NavigationMesh &nav,
                Physics const &physics);

			void generateNavMeshOld(NavigationMesh &nav,
                std::vector<Triangle> terrain,
                std::vector<StaticObject> objects) const;
            void generateNavigationMesh(NavigationMesh &nav, Physics const &physics);
		private:
            DirectX::SimpleMath::Vector3 getNormal(
                Triangle const &triangle,
                VertexOrder vertexOrder = CLOCKWISE) const;
	};
}

#endif