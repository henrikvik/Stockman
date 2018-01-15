#include <AI\Pathing\NavigationMesh.h>
#include <Engine\newd.h>

#define EPSILON 0.0001f
#define EPSILON_NODE 0.2f

using namespace Logic;

const DirectX::SimpleMath::Vector3 NavigationMesh::DOWN_Y = DirectX::SimpleMath::Vector3(0.f, -1.f, 0.f);
const std::string NavigationMesh::FILE_NAME = "Enemies/NavigationMesh";
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
    edgesList.clear();
    nodes.clear();
}
int NavigationMesh::addTriangle(Triangle const & triangle)
{
	triangleList.push_back(triangle);
    return static_cast<int> (triangleList.size()) - 1;
}

// dont use
void NavigationMesh::removeTriangle(int index)
{
    std::swap(triangleList[index], triangleList[triangleList.size() - 1]);
    triangleList.pop_back();

    std::swap(edgesList[index], edgesList[edgesList.size() - 1]);
    edgesList.pop_back();

    std::swap(nodes[index], nodes[nodes.size() - 1]);
    nodes.pop_back();

    // THIS IS VERY SLOW, BUT WHATEVER; THAT IS NOT IMPORTANT WHEN EDITING
    // remove edges
    for (auto &edges : edgesList) {
        for (int i = 0; i < edges.size(); i++) {
            if (edges[i].index == index)
            {
                printf("REmoved edge: %d\n", i);
                std::swap(edges[i], edges[edges.size() - 1]);
                edges.pop_back();
                i--;
            }
            else if (edges[i].index == edgesList.size()) // change index due to swap
            {
                edges[i].index = index;
            }
        }
    }
}

void NavigationMesh::addEdge(int from, int to)
{
    edgesList[from].push_back({ to, getNodes()[to] });
}

void NavigationMesh::addDoubleEdge(int from, int to, DirectX::SimpleMath::Vector3 edgeNode)
{
    edgesList[from].push_back({ to, edgeNode });
    edgesList[to].push_back({ from, edgeNode });
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
	return edgesList[from];
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
	}
}

void NavigationMesh::reserveEdges(size_t size)
{
    edgesList.resize(size);
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
	
	for (int j = 0; j < edgesList.size(); j++)
	{
		for (int i = 0; i < edgesList[j].size(); i++)
		{
            data->push_back({ nodes[j],                       { 0.f, 0.f, 255.f } });
            data->push_back({ edgesList[j][i].connectionNode, { 255.f, 0.f, 255.f } });
            data->push_back({ edgesList[j][i].connectionNode, { 255.f, 0.f, 255.f } });
            data->push_back({ nodes[edgesList[j][i].index],   { 0.f, 0.f, 255.f } });
		}
	}

	return data;
}

bool NavigationMesh::saveToFile(std::string fileName) const
{
    std::vector<FileLoader::LoadedStruct> data;
    FileLoader::LoadedStruct base;
    FileLoader::LoadedStruct vertexStruc;

    // base data
    base.ints["triangles"] = triangleList.size();
    base.ints["nodes"]     = nodes.size();
    base.ints["edges"]     = edgesList.size();

    data.push_back(base);
    int i = 0;

    for (auto &tri : triangleList)
    {
        FileLoader::LoadedStruct element;
        element.ints["id"] = i++; // TEST
        data.push_back(element);

        for (auto &vertex : tri.vertices)
        {
            saveVertex(vertexStruc, vertex);
            data.push_back(vertexStruc);
        }
    }

    for (auto &node : nodes)
    {
        saveVertex(vertexStruc, node);
        data.push_back(vertexStruc);
    }

    for (auto &edges : edgesList)
    {
        FileLoader::LoadedStruct baseElement, edgeElement;
        baseElement.ints["edges"] = edges.size();
        data.push_back(baseElement);

        for (auto &edge : edges)
        {
            // index connected to
            edgeElement.ints["index"] = edge.index;
            // connection
            saveVertex(edgeElement, edge.connectionNode);

            data.push_back(edgeElement);
        }
    }

    return FileLoader::singleton().saveStructsToFile(data, fileName) == 0;
}

bool NavigationMesh::loadFromFile(std::string fileName)
{
    std::vector<FileLoader::LoadedStruct> data;
    FileLoader::LoadedStruct base, temp;
    int ret = FileLoader::singleton().loadStructsFromFile(data, fileName);
    if (ret != 0) return false;

    // base data
    base = data[0];
    int triangleSize = base.ints["triangles"];
    int nodeSize     = base.ints["nodes"];
    int edgeSize     = base.ints["edges"];

    triangleList.resize(triangleSize);
    nodes.resize(nodeSize);
    edgesList.resize(edgeSize);

    int currentLine = 1;
    // load all triangles (id and three vertices)
    for (int i = 0; i < triangleSize; i++)
    {
        NavigationMesh::Triangle tri;
        FileLoader::LoadedStruct element = data[currentLine];

        currentLine++;

        // load the three vertices in the triangle
        for (int j = 0; j < TRI_VERTICES; j++)
        {
            temp = data[j + currentLine];
            loadVertex(temp, tri.vertices[j]);
        }
        currentLine += TRI_VERTICES;

        triangleList[i] = tri;
    }

    // load the nodes (just a vertex)
    for (int i = 0; i < nodeSize; i++)
    {
        loadVertex(data[i + currentLine], nodes[i]);
    }
    currentLine += nodeSize;

    // load the edges, all edges beetwen nodes and a vertex that connects the nodes
    for (int i = 0; i < edgeSize; i++)
    {
        edgesList[i].resize(data[currentLine].ints["edges"]);

        currentLine++;
        for (int j = 0; j < edgesList[i].size(); j++)
        {
            // index connected to
            edgesList[i][j].index = data[j + currentLine].ints["index"];
            // connection
            loadVertex(data[j + currentLine], edgesList[i][j].connectionNode);
        }
        currentLine += edgesList[i].size();
    }

    return true;
}

void NavigationMesh::saveVertex(FileLoader::LoadedStruct &struc, DirectX::SimpleMath::Vector3 const &vec) const
{
    struc.floats["x"] = vec.x;
    struc.floats["y"] = vec.y;
    struc.floats["z"] = vec.z;
}

void NavigationMesh::loadVertex(FileLoader::LoadedStruct &struc, DirectX::SimpleMath::Vector3 &vec) const
{
    vec.x = struc.floats["x"];
    vec.y = struc.floats["y"];
    vec.z = struc.floats["z"];
}

int NavigationMesh::getIndex(DirectX::SimpleMath::Vector3 const &pos, DirectX::SimpleMath::Vector3 const &dir) const
{
	// ray vs triangle, copied, change to own algo later, ?
	for (int i = 0; i < triangleList.size(); i++)
	{
		if (isPosOnIndex(pos, i, dir))
			return i;
	}

	return -1;
}

bool NavigationMesh::isPosOnIndex(DirectX::SimpleMath::Vector3 const &pos, int index, DirectX::SimpleMath::Vector3 const &dir) const
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
