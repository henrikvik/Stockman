#include <AI/Behavior/AStar.h>
using namespace Logic;

AStar::AStar(std::string file)
{

}

AStar::~AStar()
{

}

AStar::Node AStar::getNextNode(Entity const &enemy, Entity const &target)
{
	return { 0, target.getPositionBT() };
}

void AStar::generateNavigationMesh()
{
	PASVF pasvf;
	pasvf.generateNavMesh(navigationMesh, {}, {});
}

void AStar::generateNodesFromFile()
{

}