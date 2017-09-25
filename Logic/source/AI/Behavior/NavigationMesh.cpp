#include <AI\Behavior\NavigationMesh.h>
#include <Misc\FileLoader.h>
using namespace Logic;

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::~NavigationMesh()
{
}

void NavigationMesh::addTriangle(Triangle const & triangle)
{
	triangleList.push_back(triangle);
}

void NavigationMesh::createNodesFromTriangles()
{
	nodes.clear();
	DirectX::SimpleMath::Vector3 mid;
	for (auto const &t : triangleList)
	{
		mid = DirectX::SimpleMath::Vector3::Zero;
		for (auto const &v : t.vertices)
			mid += v;
		mid /= 3; // nr of vertices in tri
	}
	nodes.push_back(mid);
}

const std::vector<DirectX::SimpleMath::Vector3>& NavigationMesh::getNodes() const 
{
	return nodes;
}
int NavigationMesh::getIndex(DirectX::SimpleMath::Vector3 const & pos) const
{
	return 0;
}
const std::vector<NavigationMesh::Triangle>& NavigationMesh::getList() const
{
	return triangleList;
}
