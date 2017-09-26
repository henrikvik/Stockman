#include <AI\Behavior\NavigationMesh.h>
#include <Misc\FileLoader.h>
#define EPSILON 0.0001f
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
	// ray vs triangle, copied, change to own algo later, ?
	DirectX::SimpleMath::Vector3 dir = { 0, -1, 0 };
	DirectX::SimpleMath::Vector3 p, q, t;
	for (int i = 0; i < triangleList.size(); i++)
	{
		// Vectors from p1 to p2/p3 (edges)		Vector3 p, q, t;
		float det, invDet, u, v;

		//Find vectors for two edges sharing vertex/point p1
		DirectX::SimpleMath::Vector3 e1 = 
			triangleList[i].vertices[1] - triangleList[i].vertices[0];
		DirectX::SimpleMath::Vector3 e2 =
			triangleList[i].vertices[2] - triangleList[i].vertices[0];

		// calculating determinant 
		p = dir.Cross(e2);

		//Calculate determinat
		det = e1.Dot(p);

		//if determinant is near zero, ray lies in plane of triangle otherwise not
		if (det > -EPSILON && det < EPSILON) { continue; }
		invDet = 1.0f / det;

		//calculate distance from p1 to ray origin
		t = pos - triangleList[i].vertices[0];

		//Calculate u parameter
		u = t.Dot(p) * invDet;

		//Check for ray hit
		if (u < 0 || u > 1) { continue; }

		//Prepare to test v parameter
		q = t.Cross(triangleList[i].vertices[0]);

		//Calculate v parameter
		v = dir.Dot(p) * invDet;

		//Check for ray hit
		if (v < 0 || u + v > 1) { continue; }

		if ((triangleList[i].vertices[1].Dot(q) * invDet) > EPSILON)
		{
			//ray does intersect
			return i;
		}
	}

	return -1;
}
const std::vector<NavigationMesh::Triangle>& NavigationMesh::getList() const
{
	return triangleList;
}
