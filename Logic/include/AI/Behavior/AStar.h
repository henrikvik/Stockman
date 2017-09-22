#ifndef ASTAR_H
#define ASTAR_H

#include <string>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include "NavigationMesh.h"

#include <Entity\Entity.h>

namespace Logic
{
	class AStar
	{
		public:
			struct Node {
				DirectX::SimpleMath::Vector3 position;
				int triangleID;
			};
		private:
			std::string file;
			std::vector<Node> nodes; //testing
			NavigationMesh navigationMesh;
		
			void generateNodesFromFile();
		public:
			// string for the offline loaded nav mesh
			AStar(std::string file);
			~AStar();

			Node getNextNode(Entity const &enemy, Entity const &target);

			// iniate the nodes
			void init();
	};
}
#endif