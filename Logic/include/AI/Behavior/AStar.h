#ifndef ASTAR_H
#define ASTAR_H

#include <string>
#include <vector>
#include <queue>

#include "NavigationMesh.h"
#include "PASVF.h"

#include <Entity\Entity.h>
#include <Graphics\include\Renderer.h>

namespace Logic
{
	class AStar
	{
		public:
			struct Node {
				int triangleID;
				btVector3 position;
			};

			// to calc path testing rn
			struct NavNode
			{
				bool onClosedList, explored; // for testing
				int nodeIndex, parent; // index in nav mesh
				float h, g; // cost to node

				// remove later, this is unused at the moment
				bool operator<(NavNode const &other) const {
					return g + h < other.g + other.h;
				}
				bool operator>(NavNode const &other) const {
					return g + h > other.g + other.h;
				}
			};

			// singleton for the moment
			static AStar& singleton()
			{
				static AStar aStar("");
				return aStar;
			}
		private:
			std::string file;
			std::vector<NavNode> navNodes; //testing
			NavigationMesh navigationMesh;
		
			float heuristic(DirectX::SimpleMath::Vector3 &from,
				DirectX::SimpleMath::Vector3 &to) const;
			void generateNodesFromFile();
			bool nodeInQue(int index, std::priority_queue<NavNode*> que) const;
		public:
			// string for the offline loaded nav mesh
			AStar(std::string file);
			~AStar();

			Node getNextNode(Entity const &enemy, Entity const &target);
			void reconstructPath(std::vector<NavNode*> &navNodes,
				std::vector<DirectX::SimpleMath::Vector3> &nodes);
			void renderNavigationMesh(Graphics::Renderer &renderer);

			// iniate the nodes
			void generateNavigationMesh();
	};
}
#endif