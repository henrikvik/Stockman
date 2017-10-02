#include <AI/Behavior/AStar.h>
#include <stack>
#include <stdio.h> // for testing obv
#define NO_PARENT -1
using namespace Logic;

AStar::AStar(std::string file)
{
	// for testing
	generateNavigationMesh();
	targetIndex = -1;
}

AStar::~AStar()
{
	delete debugDataTri.points;
	delete debugDataEdges.points;
}

// returns nothing if on same triangle or an error occured
std::vector<const DirectX::SimpleMath::Vector3*>
	AStar::getPath(Entity const &enemy, Entity const &target)
{
	// all nodes in navMesh
	std::vector<DirectX::SimpleMath::Vector3> nodes =
		navigationMesh.getNodes();

	// reset nav nodes
	for (size_t i = 0; i < navNodes.size(); i++)
	{
		navNodes[i].onClosedList = navNodes[i].explored = false;
		navNodes[i].g = 0;
		navNodes[i].parent = NO_PARENT;
	}

	// openlist and a test offset
	auto comp = [](NavNode *fir, NavNode *sec) { return *fir > *sec; };
	std::priority_queue<NavNode*, std::vector<NavNode*>, decltype(comp)> openList(comp);
	DirectX::SimpleMath::Vector3 offset(0, 5, 0);

	// get indicies
	int startIndex = navigationMesh.getIndex(enemy.getPosition() + offset);
	// printf("StartIndex: %d, End index: %d (AStar.cpp:%d)\n", startIndex, endIndex, __LINE__);

	// test special cases
	if (startIndex == targetIndex || startIndex == -1 || targetIndex == -1)
		return { };

	navNodes[startIndex].h = heuristic(nodes[startIndex], nodes[targetIndex]);
	navNodes[startIndex].explored = true;
	openList.push(&navNodes[startIndex]);

	NavNode *currentNode = nullptr;
	NavNode *explore = nullptr;

	float f;

	while (!openList.empty())
	{
		currentNode = openList.top();
		f = currentNode->g + currentNode->h;
		openList.pop();

		for (int index : navigationMesh.getEdges(currentNode->nodeIndex))
		{
			explore = &navNodes[index];
			if (index == targetIndex) 
			{
				explore->parent = currentNode->nodeIndex;
				currentNode = explore;
				return reconstructPath(currentNode); // found node
			}

			if (!explore->explored) // Unexplored
			{
				explore->explored = true;

				explore->g = f;
				explore->h = heuristic(nodes[index], nodes[targetIndex]) * 0.1;

				explore->parent = currentNode->nodeIndex;
				openList.push(explore);
			}
			else if (explore->explored && !explore->onClosedList && explore->g > f) // in Open List
			{
				explore->g = f;

				explore->parent = currentNode->nodeIndex;
			}
			else if (explore->g > f) // in Closed List
			{
				explore->onClosedList = false;

				explore->g = f;

				explore->parent = currentNode->nodeIndex;
				openList.push(explore);
			}
		}

		currentNode->onClosedList = true;
	}

	if (!currentNode)
	{
		printf("Major Warning: A* can't find path, enemy or player is in a bad location!\nContact"
			"Lukas or something (AStar.cpp:%d)\n", __LINE__);
		return { };
	}

	return reconstructPath(currentNode);
}

std::vector<const DirectX::SimpleMath::Vector3*> AStar::reconstructPath(NavNode *endNode)
{
	// flip the list
	std::stack<const DirectX::SimpleMath::Vector3*> list;
	std::vector<const DirectX::SimpleMath::Vector3*> ret;

	if (endNode->nodeIndex != targetIndex)
	{
		printf("BAD BAD BAD, A* fooked up, contact lw (AStar.cpp:%d)\n", __LINE__);

	}

	while (endNode->parent != NO_PARENT)
	{
		list.push(&(navigationMesh.getNodes()[endNode->nodeIndex]));
		endNode = &navNodes[endNode->parent];
	}

	while (!list.empty())
	{
		ret.push_back(list.top());
		list.pop();
	}

	return ret;
}

void AStar::renderNavigationMesh(Graphics::Renderer & renderer)
{
	renderer.queueRenderDebug(&debugDataTri);
	renderer.queueRenderDebug(&debugDataEdges);
}

void AStar::loadTargetIndex(Entity const & target)
{
	targetIndex = navigationMesh.getIndex(target.getPosition());
}

void AStar::generateNavigationMesh()
{
	PASVF pasvf;
	pasvf.generateNavMesh(navigationMesh, {}, {});
	navigationMesh.createNodesFromTriangles();
	// test //
#define ROW 4
	for (size_t i = 0; i < navigationMesh.getNodes().size() - 1; i++)
	{
		if (i % ROW != 0)
		{
			navigationMesh.addEdge(i, i + 1);
			navigationMesh.addEdge(i + 1, i);
		}

		if (i < navigationMesh.getNodes().size() - ROW - 1)
		{
			navigationMesh.addEdge(i, i + ROW);
			navigationMesh.addEdge(i + ROW, i);

			if (i % ROW != 0)
			{
				navigationMesh.addEdge(i, i + ROW + 1);
				navigationMesh.addEdge(i + ROW + 1, i);
			}
		}
	}

	navNodes.clear();
	for (size_t i = 0; i < navigationMesh.getNodes().size(); i++)
		navNodes.push_back(
			{ false, false, 
			static_cast<int> (i), 0, 0 }
		);

	// debugging
	debugDataTri.color = DirectX::SimpleMath::Color(0, 1, 0);
	debugDataTri.useDepth = false;
	debugDataTri.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	debugDataTri.points = navigationMesh.getRenderDataTri();

	debugDataEdges.color = DirectX::SimpleMath::Color(0, 0, 1);
	debugDataEdges.useDepth = false;
	debugDataEdges.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	debugDataEdges.points = navigationMesh.getRenderDataEdges();
}

float AStar::heuristic(DirectX::SimpleMath::Vector3 &from,
					   DirectX::SimpleMath::Vector3 &to) const
{
	return (to - from).LengthSquared(); // faster than Length()
}

void AStar::generateNodesFromFile()
{

}

// this can maybe be optimized record most used functions to see which is needed
bool AStar::nodeInQue(int index, std::priority_queue<NavNode*> que) const
{
	while (!que.empty())
		if (que.top()->nodeIndex == index)
			return true;
		else
			que.pop();
	return false;
}
