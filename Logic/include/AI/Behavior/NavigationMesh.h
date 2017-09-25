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
			NavigationMesh(NavigationMesh const &other) = delete;
			NavigationMesh* operator=(NavigationMesh const &other) = delete;
			~NavigationMesh();

			void createTestMesh();
			const std::vector<Triangle>& getList() const;
		private:
			std::vector<Triangle> triangleList;
	};
}

#endif