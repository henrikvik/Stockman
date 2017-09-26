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
	int startIndex = navigationMesh.getIndex(enemy.getPosition()),
		endIndex = navigationMesh.getIndex(target.getPosition());
	std::vector<NavNode> openList;

	if (startIndex == -1)
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

	return { 0, target.getPositionBT() };
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