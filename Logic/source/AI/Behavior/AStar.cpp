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

void AStar::generateNodesFromFile()
{

}

void AStar::init()
{
	// generateNodesFromFile();
	navigationMesh.createTestMesh();
}