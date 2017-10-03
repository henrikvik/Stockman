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

void NavigationMesh::addEdge(int from, int to)
{
	edges[from].indices.push_back(to);
}

std::vector<int>& NavigationMesh::getEdges(int from)
{
	return edges[from].indices;
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
		nodes.push_back(mid);
		edges.push_back(Edge());
	}
}

const std::vector<DirectX::SimpleMath::Vector3>& NavigationMesh::getNodes() const 
{
	return nodes;
}

std::vector<DirectX::SimpleMath::Vector3>* NavigationMesh::getRenderDataTri() 
{
	std::vector<DirectX::SimpleMath::Vector3> *data =
		new std::vector<DirectX::SimpleMath::Vector3>();

	for (Triangle const &tri : triangleList)
		for (int i = 0; i < 3; i++)
			data->push_back(tri.vertices[i]);

	return data;
}

std::vector<DirectX::SimpleMath::Vector3>* NavigationMesh::getRenderDataEdges()
{
	std::vector<DirectX::SimpleMath::Vector3> *data =
		new std::vector<DirectX::SimpleMath::Vector3>();
	
	for (int j = 0; j < edges.size(); j++)
	{
		for (int i = 0; i < edges[j].indices.size(); i++)
		{
			data->push_back(nodes[j]);
			data->push_back(nodes[edges[j].indices[i]]);
		}
	}

	return data;
}

const std::vector<NavigationMesh::Edge>& NavigationMesh::getEdges() const
{
	return edges;
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
		q = t.Cross(e1);

		//Calculate v parameter
		v = dir.Dot(q) * invDet;

		//Check for ray hit
		if (v < 0 || u + v > 1) { continue; }

		if ((e2.Dot(q) * invDet) > EPSILON)
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
