#ifndef ASTAR_H
#define ASTAR_H

#include <string>
#include <vector>
#include <SimpleMath.h>

#include <Entity\Entity.h>

namespace Logic
{
	class AStar
	{
		public:
			struct Node {
				float x, y, z;
			};
		private:
			std::string file;
			std::vector<Node> nodes; //testing
		
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