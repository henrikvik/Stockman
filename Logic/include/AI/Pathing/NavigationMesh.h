#ifndef NAVIGATION_MESH_H
#define NAVIGATION_MESH_H

#include <d3d11.h>
#include <SimpleMath.h>

#include <Vector>

// this class should be created offline and loaded with a file
namespace Logic
{
	class NavigationMesh
	{
	    public:
            // Edges: List Of Edge. Edge = a connectionNode and the index it is connected to
            struct Edge {
                int index;
                DirectX::SimpleMath::Vector3 connectionNode;
            };
            typedef std::vector<Edge> Edges;

			struct Triangle {
				int id;
				DirectX::SimpleMath::Vector3 vertices[3];
			};

			NavigationMesh();
			NavigationMesh(NavigationMesh const &other) = delete;
			NavigationMesh* operator=(NavigationMesh const &other) = delete;
			~NavigationMesh();

            // only use for debugging, should never be used by normal gameplay
            void clear();

			int addTriangle(Triangle const &triangle);
            void addEdge(int from, int to);
            // from <--> to
            void addDoubleEdge(int from, int to, DirectX::SimpleMath::Vector3 edgeNode);
            void generateEdges();

            Edges& NavigationMesh::getEdges(int from);
			void createNodesFromTriangles();
			
			// returns int of the index that has this pos in it
			// (triangleList, getList(), index)
			int getIndex(DirectX::SimpleMath::Vector3 const &pos) const;
			bool isPosOnIndex(DirectX::SimpleMath::Vector3 const &pos, int index) const;

			const std::vector<Triangle>& getList() const;
			const std::vector<DirectX::SimpleMath::Vector3>& getNodes() const;

			std::vector<DirectX::SimpleMath::Vector3>* getRenderDataTri();
			std::vector<DirectX::SimpleMath::Vector3>* getRenderDataEdges();
		private:
			std::vector<Triangle> triangleList;
			std::vector<DirectX::SimpleMath::Vector3> nodes;
			std::vector<Edges> listEdges;

			static const DirectX::SimpleMath::Vector3 dir;
	};
}

#endif