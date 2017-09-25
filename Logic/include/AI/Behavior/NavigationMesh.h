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

			NavigationMesh();
			//NavigationMesh(NavigationMesh const &other);
			//NavigationMesh* operator=(NavigationMesh const &other);
			~NavigationMesh();

			void addTriangle(Triangle const &triangle);
			void createNodesFromTriangles();

			const std::vector<Triangle>& getList() const;
			const std::vector<DirectX::SimpleMath::Vector3>& getNodes() const;
		private:
			std::vector<Triangle> triangleList;
			std::vector<DirectX::SimpleMath::Vector3> nodes;
	};
}

#endif