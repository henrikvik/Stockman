#include <AI\Behavior\Pathing.h>
#include <Graphics\include\Renderer.h>
using namespace Logic;

Pathing::Pathing()
{
	m_currentNode = 0;
    m_debugInfo = nullptr;
}

Pathing::~Pathing()
{
    if (m_debugInfo)
    {
        if (m_debugInfo->points)
            delete m_debugInfo->points;
        delete m_debugInfo;
    }
}

void Pathing::setPath(std::vector<const DirectX::SimpleMath::Vector3*> const &path)
{
	m_currentNode = 0;
	m_path = path;
}

std::vector<const DirectX::SimpleMath::Vector3*>& Pathing::getPath()
{
	return m_path;
}

const DirectX::SimpleMath::Vector3& Pathing::getNode() const
{
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
	return m_currentNode >= m_path.size() - 1;
}

void Pathing::initDebugRendering()
{
    m_debugInfo = newd Graphics::RenderDebugInfo();
    m_debugInfo->color = DirectX::SimpleMath::Color(1, 0, 0);
    m_debugInfo->useDepth = true;
    m_debugInfo->topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
    m_debugInfo->points = newd std::vector<DirectX::SimpleMath::Vector3>();
}

void Pathing::renderDebugging(Graphics::Renderer &renderer, DirectX::SimpleMath::Vector3 &start)
{
    if (m_debugInfo->points->size() > 1)
    {
        m_debugInfo->points->clear();
        m_debugInfo->points->push_back(start);

        for (int i = 0; i < m_path.size(); i++) // this is slow, but debugging should not be when efficiency is required 
            m_debugInfo->points->push_back(*m_path[i]);

        renderer.queueRenderDebug(m_debugInfo);
    }
}