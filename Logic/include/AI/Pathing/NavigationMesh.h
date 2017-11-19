#ifndef NAVIGATION_MESH_H
#define NAVIGATION_MESH_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <Vector>

#include <Misc\FileLoader.h>

namespace Logic
{
	class NavigationMesh
	{
	    public:
            static const std::string FILE_NAME;
            static const int TRI_VERTICES = 3;

            // Edges: List Of Edge. Edge = a connectionNode and the index it is connected to
            struct Edge {
                int index;
                DirectX::SimpleMath::Vector3 connectionNode;
            };
            typedef std::vector<Edge> Edges;

			struct Triangle {
				int id;
				DirectX::SimpleMath::Vector3 vertices[TRI_VERTICES];
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
            void reserveEdges(size_t size);
			
			// returns int of the index that has this pos in it
			// (triangleList, getList(), index)
			int getIndex(DirectX::SimpleMath::Vector3 const &pos) const;
			bool isPosOnIndex(DirectX::SimpleMath::Vector3 const &pos, int index) const;

			const std::vector<Triangle>& getList() const;
			const std::vector<DirectX::SimpleMath::Vector3>& getNodes() const;

            std::vector<std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Color>>* 
                getRenderDataTri();
            std::vector<std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Color>>* 
                getRenderDataEdges();

            // saving / loading ret true on success
            bool saveToFile(std::string fileName = FILE_NAME) const;
            bool loadFromFile(std::string fileName = FILE_NAME);
		private:
			std::vector<Triangle> triangleList;
			std::vector<Edges>    edgesList;
			std::vector<DirectX::SimpleMath::Vector3> nodes;

            void saveVertex(FileLoader::LoadedStruct &struc, DirectX::SimpleMath::Vector3 const &vec) const;
            void loadVertex(FileLoader::LoadedStruct &struc, DirectX::SimpleMath::Vector3 &vec) const;

			static const DirectX::SimpleMath::Vector3 DOWN_Y;
	};
}

#endif