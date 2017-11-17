#include <AI\Pathing\NavigationMesh.h>
#include <Misc\FileLoader.h>
#include <Engine\newd.h>
#define EPSILON 0.0001f
#define EPSILON_NODE 0.2f
using namespace Logic;

const DirectX::SimpleMath::Vector3 NavigationMesh::dir = DirectX::SimpleMath::Vector3(0.f, -1.f, 0.f);
typedef std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Color> Point;

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::~NavigationMesh()
{
}

void NavigationMesh::clear()
{
    triangleList.clear();
    listEdges.clear();
    nodes.clear();
}
int NavigationMesh::addTriangle(Triangle const & triangle)
{
	triangleList.push_back(triangle);
    return static_cast<int> (triangleList.size()) - 1;
}

void NavigationMesh::addEdge(int from, int to)
{
    listEdges[from].push_back({ to, getNodes()[to] });
}

void NavigationMesh::addDoubleEdge(int from, int to, DirectX::SimpleMath::Vector3 edgeNode)
{
    listEdges[from].push_back({ to, edgeNode });
    listEdges[to].push_back({ from, edgeNode });
}

void NavigationMesh::generateEdges()
{
    // simple custom algo
    // add edge beetwen triangles that shares two vertices
    // (change to one shared vertex in future to see if it works with the map)
    int same = 0;
    for (size_t i = 0; i < triangleList.size(); i++)
    {
        for (size_t j = 0; j < triangleList.size() && j != i; j++)
        {
            same = 0;
            for (auto const &vertex : triangleList[i].vertices)
            {
                for (auto const &otherV : triangleList[j].vertices)
                {
                    if ((vertex - otherV).Length() <= EPSILON_NODE)
                    {
                        if (++same > 1)
                        {
                            addEdge(static_cast<int> (i), static_cast<int> (j));
                            addEdge(static_cast<int> (j), static_cast<int> (i));
                            break;
                        }
                    }
                }
                if (same > 1) break;
            }
        }
    }
}

NavigationMesh::Edges& NavigationMesh::getEdges(int from)
{
	return listEdges[from];
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
        listEdges.push_back({});
	}
}

const std::vector<DirectX::SimpleMath::Vector3>& NavigationMesh::getNodes() const 
{
	return nodes;
}

std::vector<Point>* NavigationMesh::getRenderDataTri() 
{
	std::vector<Point> *data = newd std::vector<Point>();

    for (size_t t = 0; t < triangleList.size(); t++)
    {
        for (int i = 0; i < 3; i++) // List of lines to make a triangle
        {
            data->push_back({ triangleList[t].vertices[i], 
                DirectX::SimpleMath::Color( (t * t) % 255 / 255.f, (t + t) % 255 / 255.f, (t + t * 2) % 255 / 255.f ) });
            data->push_back({triangleList[t].vertices[i == 2 ? 0 : i + 1], 
                DirectX::SimpleMath::Color( (t * t) % 255 / 255.f, (t + t) % 255 / 255.f, (t + t * 2) % 255 / 255.f ) });
        }
    }

	return data;
}

std::vector<Point>* NavigationMesh::getRenderDataEdges()
{
    std::vector<Point> *data = newd std::vector<Point>();

    for (auto node : nodes)
    {
     //   data->push_back(node + DirectX::SimpleMath::Vector3{ 0, 2.f, 0 });
    //    data->push_back(node - DirectX::SimpleMath::Vector3{ 0, 2.f, 0 });
    }
	
	for (int j = 0; j < listEdges.size(); j++)
	{
		for (int i = 0; i < listEdges[j].size(); i++)
		{
            data->push_back({ nodes[j],                       { 0.f, 0.f, 255.f } });
            data->push_back({ listEdges[j][i].connectionNode, { 255.f, 0.f, 255.f } });
            data->push_back({ listEdges[j][i].connectionNode, { 255.f, 0.f, 255.f } });
            data->push_back({ nodes[listEdges[j][i].index],   { 0.f, 0.f, 255.f } });
		}
	}

	return data;
}

int NavigationMesh::getIndex(DirectX::SimpleMath::Vector3 const &pos) const
{
	// ray vs triangle, copied, change to own algo later, ?
	for (int i = 0; i < triangleList.size(); i++)
	{
		if (isPosOnIndex(pos, i))
			return i;
	}

	return -1;
}

bool NavigationMesh::isPosOnIndex(DirectX::SimpleMath::Vector3 const & pos, int index) const
{
	Triangle tri = triangleList[index];
	DirectX::SimpleMath::Vector3 p, q, t;
	// Vectors from p1 to p2/p3 (edges)		Vector3 p, q, t;
	float det, invDet, u, v;

	//Find vectors for two edges sharing vertex/point p1
	DirectX::SimpleMath::Vector3 e1 =
		tri.vertices[1] - tri.vertices[0];
	DirectX::SimpleMath::Vector3 e2 =
		tri.vertices[2] - tri.vertices[0];

	// calculating determinant 
	p = dir.Cross(e2);

	//Calculate determinat
	det = e1.Dot(p);

	//if determinant is near zero, ray lies in plane of triangle otherwise not
	if (det > -EPSILON && det < EPSILON) { return false; }
	invDet = 1.0f / det;

	//calculate distance from p1 to ray origin
	t = pos - tri.vertices[0];

	//Calculate u parameter
	u = t.Dot(p) * invDet;

	//Check for ray hit
	if (u < 0 || u > 1) { return false; }

	//Prepare to test v parameter
	q = t.Cross(e1);

	//Calculate v parameter
	v = dir.Dot(q) * invDet;

	//Check for ray hit
	if (v < 0 || u + v > 1) { return false; }

	if ((e2.Dot(q) * invDet) > EPSILON)
	{
		//ray does intersect
		return true;
	}
	return false;
}
const std::vector<NavigationMesh::Triangle>& NavigationMesh::getList() const
{
	return triangleList;
}
