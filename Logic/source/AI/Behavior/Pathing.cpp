#include <AI\Behavior\Pathing.h>
#include <Graphics\include\Renderer.h>
using namespace Logic;

Pathing::Pathing()
{
	m_currentNode = 0;
	m_debugInfo.points = nullptr;
}

void Pathing::setPath(std::vector<const DirectX::SimpleMath::Vector3*> const &path)
{
	m_currentNode = 0;
	m_path = path;

	if (m_debugInfo.points)
		delete m_debugInfo.points;
}

std::vector<const DirectX::SimpleMath::Vector3*>& Logic::Pathing::getPath()
{
	return m_path;
}

const DirectX::SimpleMath::Vector3& Pathing::getNode() const
{
	if (m_path.empty()) return { 0,0,0 }; // for testing
	return *m_path[m_currentNode];
}

void Pathing::setCurrentNode(int currentNode)
{
	m_currentNode = currentNode;
}

int Pathing::getCurrentNode() const
{
	return m_currentNode;
}

bool Pathing::pathIsEmpty() const
{
	return m_path.empty();
}

bool Pathing::pathOnLastNode() const
{
	return m_currentNode < m_path.size() - 1;
}

void Pathing::initDebugRendering()
{
	m_debugInfo.color = DirectX::SimpleMath::Color(1, 0, 0);
	m_debugInfo.useDepth = true;
	m_debugInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	m_debugInfo.points = new std::vector<DirectX::SimpleMath::Vector3>();
}

void Pathing::renderDebugging(Graphics::Renderer &renderer)
{
	renderer.queueRenderDebug(&m_debugInfo);
}