#ifndef ASTAR_H
#define ASTAR_H

#include <Graphics\include\RenderQueue.h>

#include <string>
#include <vector>
#include <queue>

#include "NavigationMesh.h"
#include "NavigationMeshGeneration.h"

#include <Entity\Entity.h>

namespace Logic
{
    class Physics;
	class AStar
	{
		public:
			struct Node {
				int triangleID;
				btVector3 position;
			};

			// debugging
            NewDebugRenderInfo debugDataTri, debugDataEdges;

			// to calc path testing rn
			struct NavNode
			{
				bool onClosedList, explored; // for testing
				int nodeIndex, parent; // index in nav mesh
                DirectX::SimpleMath::Vector3 *connectionNode; // pointer to the connection node beetwen this <--> parent, this is a temporary solution  (?)
				float h, g; // cost to node

				bool operator<(NavNode const &other) const {
					return g + h < other.g + other.h;
				}
				bool operator>(NavNode const &other) const {
					return g + h > other.g + other.h;
				}
			};

			static AStar& singleton()
			{
				static AStar aStar("");
				return aStar;
			}
		private:
            static const int NULL_NODE;

			std::string file;
			std::vector<NavNode> navNodes; //testing

			NavigationMesh navigationMesh;
            NavigationMeshGeneration generator;

            bool renderDebugTri, renderDebugEdges;

			int targetIndex; // save the triangle id to share beetwen path loading
            bool targetOutOfBounds;
		
			float heuristic(DirectX::SimpleMath::Vector3 const &from,
				DirectX::SimpleMath::Vector3 const &to) const;
			void generateNodesFromFile();
			void setupDebugging();

			bool nodeInQue(int index, std::priority_queue<NavNode*> que) const;

            void initDebugRendering();
            void createNodes();
		public:
			// string for the offline loaded nav mesh
			AStar(std::string file);
			AStar(AStar const &other) = delete;
			AStar* operator=(AStar const &other) = delete;
			~AStar();

			std::vector<const DirectX::SimpleMath::Vector3*>
				getPath(Entity const &enemy, Entity const &target);
			std::vector<const DirectX::SimpleMath::Vector3*>
				getPath(int fromIndex, int toIndex);
			std::vector<const DirectX::SimpleMath::Vector3*>
				getPath(int fromIndex);

			std::vector<const DirectX::SimpleMath::Vector3*> reconstructPath(NavNode const *endNode,
                std::vector<NavNode> const &navNodes, int toIndex);

			void renderNavigationMesh();
			// load the target triangle once per frame instead of once per path load
			void loadTargetIndex(Entity const &target);
			int getTargetIndex();

            // Mesh stuff
            int getIndex(Entity const &entity) const;
            int getIndex(btVector3 const &vec) const;
			int isEntityOnIndex(Entity const &entity, int index) const;
			size_t getNrOfPolygons() const;

			// iniate the nodes
			void generateNavigationMesh(Physics &physics);

            // debugging
            bool isRenderingDebugTri() const;
            bool isRenderingDebugEdges() const;
	};
}
#endif