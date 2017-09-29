#include <AI/Behavior/AStar.h>
#include <queue>
#include <stdio.h> // for testing obv
using namespace Logic;

AStar::AStar(std::string file)
{
	// for testing
	generateNavigationMesh();
}

AStar::~AStar()
{

}

AStar::Node AStar::getNextNode(Entity const &enemy, Entity const &target)
{
	// all nodes in navMesh
	std::vector<DirectX::SimpleMath::Vector3> nodes =
		navigationMesh.getNodes();

	// reset nav nodes
	for (int i = 0; i < navNodes.size(); i++)
	{
		navNodes[i].onClosedList = navNodes[i].explored = false;
		navNodes[i].h = navNodes[i].g = 0;
		navNodes[i].parent = 0;
	}

	// openlist and a test offset
	std::priority_queue<NavNode*> openList;
	DirectX::SimpleMath::Vector3 offset(0, 5, 0);

	// get indicies
	int startIndex = navigationMesh.getIndex(enemy.getPosition() + offset),
		endIndex = navigationMesh.getIndex(target.getPosition() + offset);
	printf("StartIndex: %d, End index: %d (AStar.cpp:%d)\n", startIndex, endIndex, __LINE__);

	// test special cases
	if (startIndex == -1 || endIndex == -1 || startIndex == endIndex)
		return { 0, target.getPositionBT() };

	navNodes[startIndex].h = heuristic(nodes[startIndex], nodes[endIndex]);
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
			if (!explore->explored) // Unexplored
			{
				explore->explored = true;

				explore->g = f;
				explore->h = heuristic(nodes[index], nodes[endIndex]);

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
			"Lukas or something (AStar.cpp:%s)\n", __LINE__);
		return { 0, {nodes[endIndex].x, nodes[endIndex].y, nodes[endIndex].z} };
	}

	DirectX::SimpleMath::Vector3 node = nodes[currentNode->nodeIndex];
	return { 0, { node.x, node.y, node.z } };
}

void AStar::reconstructPath(std::vector<NavNode*>& navNodes, std::vector<DirectX::SimpleMath::Vector3>& nodes)
{
}

void AStar::renderNavigationMesh(Graphics::Renderer & renderer)
{
	Graphics::RenderInfo info;
}

void AStar::generateNavigationMesh()
{
	PASVF pasvf;
	pasvf.generateNavMesh(navigationMesh, {}, {});
	navigationMesh.createNodesFromTriangles();
	// test
	navigationMesh.addEdge(0, 1);
	navigationMesh.addEdge(1, 0);

	navNodes.clear();
	for (int i = 0; i < navigationMesh.getNodes().size(); i++)
		navNodes.push_back(
			{ false, false, 
			i, 0, 0 }
		);
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
