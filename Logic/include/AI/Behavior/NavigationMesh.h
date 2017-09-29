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
			struct Triangle {
				int id;
				DirectX::SimpleMath::Vector3 vertices[3];
			};
			struct Edge {
				std::vector<int> indices;
			};

			NavigationMesh();
			//NavigationMesh(NavigationMesh const &other);
			//NavigationMesh* operator=(NavigationMesh const &other);
			~NavigationMesh();

			void addTriangle(Triangle const &triangle);
			void addEdge(int from, int to);
			std::vector<int>& getEdges(int from);
			void createNodesFromTriangles();
			
			// returns int of the index that has this pos in it
			// (triangleList, getList(), index)
			int getIndex(DirectX::SimpleMath::Vector3 const &pos) const;

			const std::vector<Triangle>& getList() const;
			const std::vector<DirectX::SimpleMath::Vector3>& getNodes() const;
			const std::vector<Edge>& getEdges() const;
		private:
			std::vector<Triangle> triangleList;
			std::vector<DirectX::SimpleMath::Vector3> nodes;
			std::vector<Edge> edges;
	};
}

#endif