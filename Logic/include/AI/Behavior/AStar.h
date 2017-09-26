#ifndef ASTAR_H
#define ASTAR_H

#include <string>
#include <vector>

#include "NavigationMesh.h"
#include "PASVF.h"

#include <Entity\Entity.h>

namespace Logic
{
	class AStar
	{
		public:
			struct Node {
				int triangleID;
				btVector3 position;
			};

			// singleton for the moment
			static AStar& singleton()
			{
				static AStar aStar("");
				return aStar;
			}
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
			void generateNavigationMesh();
	};
}
#endif