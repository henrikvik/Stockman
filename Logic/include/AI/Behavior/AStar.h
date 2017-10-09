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

			// debugging
			Graphics::RenderDebugInfo debugDataTri, debugDataEdges;

			// to calc path testing rn
			struct NavNode
			{
				bool onClosedList, explored; // for testing
				int nodeIndex, parent; // index in nav mesh
				float h, g; // cost to node

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
			int targetIndex; // save the triangle id to share beetwen path loading
		
			float heuristic(DirectX::SimpleMath::Vector3 &from,
				DirectX::SimpleMath::Vector3 &to) const;
			void generateNodesFromFile();
			void setupDebugging();
			bool nodeInQue(int index, std::priority_queue<NavNode*> que) const;
		public:
			// string for the offline loaded nav mesh
			AStar(std::string file);
			AStar(AStar const &other) = delete;
			AStar* operator=(AStar const &other) = delete;
			~AStar();

			std::vector<const DirectX::SimpleMath::Vector3*>
				getPath(Entity const &enemy, Entity const &target);

			std::vector<const DirectX::SimpleMath::Vector3*> 
				reconstructPath(NavNode *endNode);

			void renderNavigationMesh(Graphics::Renderer &renderer);
			// load the target triangle once per frame instead of once per path load
			void loadTargetIndex(Entity const &target);

			int getIndex(Entity const &entity) const;
			size_t getNrOfPolygons() const;

			// iniate the nodes
			void generateNavigationMesh();
	};
}
#endif