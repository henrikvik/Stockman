#include <AI/Behavior/AStar.h>
#include <queue>
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

	// test special cases
	if (startIndex == -1 || endIndex == -1 || startIndex == endIndex)
		return { 0, target.getPositionBT() };

	// open, nodeIndex and g
	openList.push(
		{ true, startIndex,
		heuristic(nodes[startIndex], nodes[endIndex]) }
	);

	NavNode currentNode;
	while (!openList.empty())
	{
		currentNode = openList.top();
		openList.pop();

		for (int index : navigationMesh.getEdges(currentNode.nodeIndex))
			openList.push(
				{ true, index,
				heuristic(nodes[index], nodes[endIndex]) }
			);

		open[currentNode.nodeIndex] = false;
	}

	return { 0, {nodes[endIndex].x, nodes[endIndex].y, nodes[endIndex].z } };
}

void AStar::generateNavigationMesh()
{
	PASVF pasvf;
	pasvf.generateNavMesh(navigationMesh, {}, {});
	navigationMesh.createNodesFromTriangles();
}

float AStar::heuristic(DirectX::SimpleMath::Vector3 &from,
					   DirectX::SimpleMath::Vector3 &to) const
{
	return (to - from).LengthSquared(); // faster than Length()
}

void AStar::generateNodesFromFile()
{

}