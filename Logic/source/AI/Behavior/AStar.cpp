#include <AI/Behavior/AStar.h>

#include <stack>
#include <stdio.h> // for testing obv
#include <cmath>

#include <Engine\Profiler.h>
#include <Engine\DebugWindow.h>
#define NO_PARENT -1
using namespace Logic;

AStar::AStar(std::string file)
{
	debugDataTri.points = nullptr;
	debugDataEdges.points = nullptr;
	// for testing
	targetIndex = 0;
    renderDebug = false;

	for (size_t i = 0; i < navNodes.size(); i++)
	{
		navNodes[i].onClosedList = navNodes[i].explored = false;
		navNodes[i].g = navNodes[i].h = 0;
		navNodes[i].nodeIndex = (int)i;
		navNodes[i].parent = NO_PARENT;
	}
}

AStar::~AStar()
{
	if (debugDataTri.points)
		delete debugDataTri.points;
	if (debugDataEdges.points)
		delete debugDataEdges.points;
}

// returns nothing if on same triangle or an error occured
std::vector<const DirectX::SimpleMath::Vector3*>
	AStar::getPath(Entity const &enemy, Entity const &target)
{
	DirectX::SimpleMath::Vector3 offset(0, 5, 0);
	int startIndex = navigationMesh.getIndex(enemy.getPosition() + offset);
	int targetIndex = navigationMesh.getIndex(target.getPosition() + offset);
	return getPath(startIndex, targetIndex);
}

std::vector<const DirectX::SimpleMath::Vector3*> AStar::getPath(int startIndex, int toIndex)
{
	// Edge cases 
	if (startIndex == toIndex || startIndex == -1 || toIndex == -1)
		return {};

	// all nodes in navMesh
	const std::vector<DirectX::SimpleMath::Vector3> &nodes = navigationMesh.getNodes();
	std::vector<AStar::NavNode> navNodes = this->navNodes;

	// openlist and a test offset
	auto comp = [](NavNode *fir, NavNode *sec) { return *fir > *sec; };
	std::priority_queue<NavNode*, std::vector<NavNode*>, decltype(comp)> openList(comp);

	navNodes[startIndex].h = heuristic(nodes[startIndex], nodes[toIndex]);
	navNodes[startIndex].explored = true;
	openList.push(&navNodes[startIndex]);

	NavNode *currentNode = nullptr;
	NavNode *explore = nullptr;

	float f;
	int index;

	while (!openList.empty())
	{
		currentNode = openList.top();
		f = currentNode->g + currentNode->h;
		openList.pop();

		for (size_t i = 0; i < navigationMesh.getEdges(currentNode->nodeIndex).size(); i++)
		{
			index = navigationMesh.getEdges(currentNode->nodeIndex)[i];
			explore = &navNodes[index];

			if (index == toIndex) // Node Found
			{
				explore->parent = currentNode->nodeIndex;
				return reconstructPath(explore, navNodes, toIndex);
			}

			if (!explore->explored) // Unexplored
			{
				explore->explored = true;

				explore->g = f;
				explore->h = heuristic(nodes[index], nodes[toIndex]) * 0.1f;

				explore->parent = currentNode->nodeIndex;
				openList.push(explore);
			} 
			else if (explore->g > f) // If path to explore is LONGER than path to it now then set it again
			{
				if (!explore->onClosedList) // in Open List
				{
					explore->g = f;

					explore->parent = currentNode->nodeIndex;
				}
				else // in Closed List
				{
					explore->onClosedList = false;

					explore->g = f;

					explore->parent = currentNode->nodeIndex;
					openList.push(explore);
				}
			}
		}

		currentNode->onClosedList = true;
	}

	if (!currentNode)
	{
		printf("Major Warning: A* can't find path, enemy or player is in a bad location!\nContact"
			"Lukas or something (AStar.cpp:%d)\n", __LINE__);
		return {};
	}

	return reconstructPath(currentNode, navNodes, toIndex);
}

std::vector<const DirectX::SimpleMath::Vector3*> AStar::getPath(int fromIndex)
{
	return getPath(fromIndex, targetIndex);
}

std::vector<const DirectX::SimpleMath::Vector3*> AStar::reconstructPath(NavNode const *endNode, std::vector<NavNode> const &navNodes, int toIndex)
{
	std::vector<const DirectX::SimpleMath::Vector3*> list;

    do {
        list.push_back(&(navigationMesh.getNodes()[endNode->nodeIndex]));
		endNode = &navNodes[endNode->parent];
    } while (endNode->parent != NO_PARENT);

    std::reverse(list.begin(), list.end());
	return list;
}

void AStar::renderNavigationMesh(Graphics::Renderer & renderer)
{
    if (renderDebug)
    {
        renderer.queueRenderDebug(&debugDataTri);
        renderer.queueRenderDebug(&debugDataEdges);
    }
}

void AStar::loadTargetIndex(Entity const & target)
{
	if (targetIndex == -1 || !isEntityOnIndex(target, targetIndex))
		targetIndex = navigationMesh.getIndex(target.getPosition());
}

int AStar::getTargetIndex()
{
	return targetIndex;
}

int AStar::getIndex(Entity const & entity) const
{
	return navigationMesh.getIndex(entity.getPosition());
}

int AStar::isEntityOnIndex(Entity const & entity, int index) const
{
	return navigationMesh.isPosOnIndex(entity.getPosition(), index);
}

size_t AStar::getNrOfPolygons() const
{
	return navigationMesh.getNodes().size();
}

void AStar::generateNavigationMesh(Physics &physics)
{
    generator.registerGenerationCommand(navigationMesh, physics);
    generator.generateNavigationMesh(navigationMesh, physics);
    DebugWindow::getInstance()->registerCommand("AI_TOGGLE_DEBUG",
        [&](std::vector<std::string> para) -> std::string {
        // test //
        /*
            Basicly, divided by two because there is two triangles per square,
            then square root because it is a square matrix (rows = columns).
            Then times two again because one row = number of squares * 2.
        */
        renderDebug = !renderDebug;
        if (!renderDebug) return "AI Debug Disabled";

        navNodes.clear();
        for (size_t i = 0; i < navigationMesh.getNodes().size(); i++)
            navNodes.push_back(
                { false, false,
                static_cast<int> (i), -1, 0, 0 }
            );

        setupDebugging();
        return "AI Debug On";
    });
}

float AStar::heuristic(DirectX::SimpleMath::Vector3 const &from,
					   DirectX::SimpleMath::Vector3 const &to) const
{
	return (to - from).LengthSquared(); // faster than Length()
}

void AStar::generateNodesFromFile()
{

}

void AStar::setupDebugging()
{
	debugDataTri.color = DirectX::SimpleMath::Color(0, 1, 0);
	debugDataTri.useDepth = false;
	debugDataTri.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	debugDataTri.points = navigationMesh.getRenderDataTri();

	debugDataEdges.color = DirectX::SimpleMath::Color(0, 0, 1);
	debugDataEdges.useDepth = false;
	debugDataEdges.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	debugDataEdges.points = navigationMesh.getRenderDataEdges();
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
