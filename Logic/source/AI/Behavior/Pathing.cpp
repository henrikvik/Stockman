#include <AI\Behavior\Pathing.h>
using namespace Logic;

Pathing::Pathing()
{
	m_currentNode = 0;
}

void Pathing::setPath(std::vector<const DirectX::SimpleMath::Vector3*> const &path)
{
	m_currentNode = 0;
	m_path = path;
}

std::vector<const DirectX::SimpleMath::Vector3*>& Logic::Pathing::getPath()
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
	return m_currentNode < m_path.size() - 1;
}