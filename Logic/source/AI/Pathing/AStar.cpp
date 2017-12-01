#include <AI/Pathing/AStar.h>

#include <stack>
#include <stdio.h> // for testing obv
#include <cmath>

#include <Engine\Profiler.h>
#include <Engine\DebugWindow.h>

using namespace Logic;
const int AStar::NULL_NODE = -1;
const DirectX::SimpleMath::Vector3 AStar::OFFSET = DirectX::SimpleMath::Vector3(0.f, 10.f, 0.f);

AStar::AStar(std::string file)
{
    targetOutOfBounds = false;

    generateNodesFromFile();

    initDebugRendering();
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
    int startIndex = navigationMesh.getIndex(enemy.getPosition() + OFFSET);
    int targetIndex = navigationMesh.getIndex(target.getPosition() + OFFSET);
    return getPath(startIndex, targetIndex);
}

std::vector<const DirectX::SimpleMath::Vector3*> AStar::getPath(int startIndex, int toIndex)
{
    // Edge cass 
    if (startIndex == toIndex || startIndex == NULL_NODE || toIndex == NULL_NODE)
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

    while (!openList.empty())
    {
        currentNode = openList.top();
        f = currentNode->g + currentNode->h;
        openList.pop();

        for (size_t i = 0; i < navigationMesh.getEdges(currentNode->nodeIndex).size(); i++)
        {
            NavigationMesh::Edge &edge = navigationMesh.getEdges(currentNode->nodeIndex)[i];
            explore = &navNodes[edge.index];

            if (edge.index == toIndex) // Node Found
            {
                explore->parent = currentNode->nodeIndex;
                explore->connectionNode = &edge.connectionNode;
                return reconstructPath(explore, navNodes, toIndex);
            }

            if (!explore->explored) // Unexplored
            {
                explore->explored = true;

                explore->g = f;
                explore->h = heuristic(nodes[edge.index], nodes[toIndex]) * 0.1f;

                explore->parent = currentNode->nodeIndex;
                explore->connectionNode = &edge.connectionNode;

                openList.push(explore);
            }
            else if (explore->g > f) // If path to explore is LONGER than path to it now then set it again
            {
                if (!explore->onClosedList) // in Open List
                {
                    explore->g = f;

                    explore->parent = currentNode->nodeIndex;
                    explore->connectionNode = &edge.connectionNode;
                }
                else // in Closed List
                {
                    explore->onClosedList = false;

                    explore->g = f;

                    explore->parent = currentNode->nodeIndex;
                    explore->connectionNode = &edge.connectionNode;

                    openList.push(explore);
                }
            }
        }

        currentNode->onClosedList = true;
    }

    if (!currentNode || currentNode->parent == NULL_NODE)
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

std::vector<const DirectX::SimpleMath::Vector3*> AStar::reconstructPath(NavNode const *endNode,
    std::vector<NavNode> const &navNodes, int toIndex)
{
    std::vector<const DirectX::SimpleMath::Vector3*> list;

    list.push_back(&(navigationMesh.getNodes()[endNode->nodeIndex]));
    while ((endNode = &navNodes[endNode->parent])->parent != NULL_NODE)
        list.push_back(endNode->connectionNode);

    std::reverse(list.begin(), list.end());
    return list;
}

void AStar::renderNavigationMesh()
{
    if (debugDataTri.points && renderDebugTri)      QueueRender(debugDataTri);
    if (debugDataEdges.points && renderDebugEdges)  QueueRender(debugDataEdges);
}

void AStar::loadTargetIndex(Entity const &target)
{
    int newIndex;
    auto &targetPos = target.getPosition();

    if ((lastTargetPosition - targetPos).LengthSquared() < 1.f)
        return;
    lastTargetPosition = targetPos;

    if (targetOutOfBounds || !isEntityOnIndex(target, targetIndex))
    {
        newIndex = navigationMesh.getIndex(target.getPosition() + OFFSET);
        if (newIndex == NULL_NODE) // if out of bounds use last index
        {
            targetOutOfBounds = true;
        }
        else
        {
            targetOutOfBounds = false;
            targetIndex = newIndex;
        }
    }
}

int AStar::getTargetIndex()
{
    return targetIndex;
}

int AStar::getIndex(Entity const &entity) const
{
    return getIndex(entity.getPositionBT());
}

int AStar::getIndex(btVector3 const &vec) const
{
    return navigationMesh.getIndex(DirectX::SimpleMath::Vector3(vec) + OFFSET);
}

int AStar::isEntityOnIndex(Entity const &entity, int index) const
{
    return navigationMesh.isPosOnIndex(entity.getPosition() + OFFSET, index);
}

size_t AStar::getNrOfPolygons() const
{
    return navigationMesh.getNodes().size();
}

void AStar::generateNavigationMesh(Physics &physics)
{
    generator.registerGenerationCommand(navigationMesh, physics);
    navigationMesh.loadFromFile();

    DebugWindow::getInstance()->registerCommand("AI_TOGGLE_DEBUG_TRI",
        [&](std::vector<std::string> para) -> std::string {
        renderDebugTri = !renderDebugTri;

        if (!renderDebugTri) return "AI Debug (Triangles) Disabled";
        else
        {
            setupDebugging();
            return "AI Debug (triangles) On";
        }
    });

    DebugWindow::getInstance()->registerCommand("AI_TOGGLE_DEBUG_EDGE",
        [&](std::vector<std::string> para) -> std::string {
        renderDebugEdges = !renderDebugEdges;

        if (!renderDebugEdges) return "AI Debug (edges) Disabled";
        else
        {
            setupDebugging();
            return "AI Debug (edges) On";
        }
    });

    DebugWindow::getInstance()->registerCommand("AI_NAV_SAVE",
        [&](std::vector<std::string> para) -> std::string {
        bool success;
        if (para.size() > 0)
            success = navigationMesh.saveToFile(para[0]);
        else
            success = navigationMesh.saveToFile();
        return success ? "Navigation mesh saved correctly! :)" : "Wenn du das siehst, fuck";
    });

    DebugWindow::getInstance()->registerCommand("AI_NAV_LOAD",
        [&](std::vector<std::string> para) -> std::string {
        bool success;
        if (para.size() > 0)
            success = navigationMesh.loadFromFile(para[0]);
        else
            success = navigationMesh.loadFromFile();

        if (success)
            createNodes();
        return success ? "Navigation mesh loaded correctly! :)" : "Wenn du das siehst, fuck";
    });

    createNodes();
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
    debugDataTri.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    debugDataTri.points = reinterpret_cast<std::vector<NewDebugRenderInfo::Point>*>
        (navigationMesh.getRenderDataTri()); // this is cheesy


    debugDataEdges.color = DirectX::SimpleMath::Color(0, 0, 1);
    debugDataEdges.useDepth = false;
    debugDataEdges.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    debugDataEdges.points = reinterpret_cast<std::vector<NewDebugRenderInfo::Point>*>
        (navigationMesh.getRenderDataEdges()); // this is cheesy;
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

bool AStar::isRenderingDebugTri() const
{
    return renderDebugTri;
}

bool AStar::isRenderingDebugEdges() const
{
    return renderDebugEdges;
}

void AStar::initDebugRendering()
{
    debugDataTri.points = nullptr;
    debugDataEdges.points = nullptr;
    // for testing
    targetIndex = 0;
    renderDebugTri = false;
    renderDebugEdges = false;
}

void AStar::createNodes()
{
    NavNode node;

    node.onClosedList = node.explored = false;
    node.g = node.h = 0;
    node.parent = NULL_NODE;
    node.connectionNode = nullptr;

    navNodes.clear();
    for (size_t t = 0; t < navigationMesh.getNodes().size(); t++)
    {
        node.nodeIndex = static_cast<int> (t);
        navNodes.push_back(node);
    }
}