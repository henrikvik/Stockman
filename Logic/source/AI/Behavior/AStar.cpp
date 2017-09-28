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
		navNodes[i].onClosedList = false;
		navNodes[i].h = navNodes[i].g = 0;
	}

	// openlist and a test offset
	auto comp = [](int a, int b) { return navNodes[a] > navNodes[b]; };
	std::priority_queue<int> openList;
	DirectX::SimpleMath::Vector3 offset(0, 5, 0);

	// get indicies
	int startIndex = navigationMesh.getIndex(enemy.getPosition() + offset),
		endIndex = navigationMesh.getIndex(target.getPosition() + offset);
	printf("StartIndex: %d, End index: %d (AStar.cpp:%d)\n", startIndex, endIndex, __LINE__);

	// test special cases
	if (startIndex == -1 || endIndex == -1 || startIndex == endIndex)
		return { 0, target.getPositionBT() };

	navNodes[startIndex].h = heuristic(nodes[startIndex], nodes[endIndex]);
	openList.push(startIndex);

	int currentNode;
	float f;
	while (!openList.empty())
	{
		currentNode = openList.top();
		f = navNodes[currentNode].g + navNodes[currentNode].h;
		openList.pop();

		for (int index : navigationMesh.getEdges(currentNode))
			if (!navNodes[index].onClosedList)
			{
				navNodes[index].g = f;
				navNodes[index].h = heuristic(nodes[index], nodes[endIndex]);
				openList.push(index);
			}
			else if (nodeOnList(index, openList.))
			{

			}
			else if (navNodes[index].g < f)
			{
				navNodes[index].g = f;
				openList.push(index);
			}

		navNodes[currentNode].onClosedList = true;
	}

	DirectX::SimpleMath::Vector3 node = nodes[currentNode];
	return { 0, { node.x, node.y, node.z } };
}

void AStar::reconstructPath(std::vector<NavNode>& navNodes, std::vector<DirectX::SimpleMath::Vector3>& nodes)
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
			{ true, i,
			0, 0 }
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

bool AStar::nodeInQue(int index, std::priority_queue<int> const &que) const
{
	for (int i = 0; i < que.size(); i++)
		if (que[i] == index)
			return true;
	return false;
}
