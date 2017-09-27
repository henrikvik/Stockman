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
	std::vector<bool> open; // insert in the nodes list later?
	open.resize(nodes.size());

	// openlist and a test offset
	std::priority_queue<NavNode> openList;
	DirectX::SimpleMath::Vector3 offset(0, 5, 0);

	// get indicies
	int startIndex = navigationMesh.getIndex(enemy.getPosition() + offset),
		endIndex = navigationMesh.getIndex(target.getPosition() + offset);
	printf("End index: %d (AStar:%d)\n", endIndex, __LINE__);

	// test special cases
	if (startIndex == -1 || endIndex == -1 || startIndex == endIndex)
		return { 0, target.getPositionBT() };

	// open, nodeIndex and g
	openList.push(
		{ true, startIndex, 0,
		heuristic(nodes[startIndex], nodes[endIndex]) }
	);

	NavNode currentNode;
	while (!openList.empty())
	{
		currentNode = openList.top();
		openList.pop();

		for (int index : navigationMesh.getEdges(currentNode.nodeIndex))
			if (!open[index])
				openList.push(
					{ true, index,
					currentNode.g + heuristic(nodes[index], nodes[endIndex]) }
				);

		open[currentNode.nodeIndex] = false;
	}

	DirectX::SimpleMath::Vector3 node = nodes[currentNode.nodeIndex];
	return { 0, { node.x, node.y, node.z } };
}

void AStar::reconstructPath(std::vector<NavNode>& navNodes, std::vector<DirectX::SimpleMath::Vector3>& nodes)
{
}

void AStar::generateNavigationMesh()
{
	PASVF pasvf;
	pasvf.generateNavMesh(navigationMesh, {}, {});
	navigationMesh.createNodesFromTriangles();
	// test
	navigationMesh.addEdge(0, 1);
	navigationMesh.addEdge(1, 0);
}

float AStar::heuristic(DirectX::SimpleMath::Vector3 &from,
					   DirectX::SimpleMath::Vector3 &to) const
{
	return (to - from).LengthSquared(); // faster than Length()
}

void AStar::generateNodesFromFile()
{

}