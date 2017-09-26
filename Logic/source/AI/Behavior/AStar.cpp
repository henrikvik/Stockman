#include <AI/Behavior/AStar.h>
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

	// use getIndex in navMesh l8
	std::vector<NavNode> openList;
	DirectX::SimpleMath::Vector3 offset(0, 5, 0);
	int startIndex = navigationMesh.getIndex(enemy.getPosition() + offset),
		endIndex = navigationMesh.getIndex(target.getPosition() + offset);

	if (startIndex == -1 || endIndex == -1)
		return { 0, target.getPositionBT() };

	// open, nodeIndex and g
	openList.push_back(
		{ true, startIndex,
		heuristic(nodes[startIndex], nodes[endIndex]) }
	);

	while (false)
	{
		// a star to path ezpz
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