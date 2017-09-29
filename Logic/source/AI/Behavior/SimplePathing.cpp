#include <AI\Behavior\SimplePathing.h>
#include <AI\Behavior\AStar.h>
using namespace Logic;

SimplePathing::SimplePathing()
{
	m_currentNode = -1;
}

const DirectX::SimpleMath::Vector3 * SimplePathing::getNode() const
{
	return m_path[m_currentNode];
}

void SimplePathing::setCurrentNode(int currentNode)
{
	m_currentNode = currentNode;
}

const btVector3 SimplePathing::updateAndReturnCurrentNode(Entity const & from, Entity const & to)
{
	/*if (pastLastNode() || pathIsEmpty())*/
	{
		loadPath(from, to);
		m_currentNode = 0;
	}

	btVector3 node;
	if (pathIsEmpty())
		node = to.getPositionBT();
	else {
		const DirectX::SimpleMath::Vector3 *vec = getNode();
		node = { vec->x, vec->y, vec->z };
	}

	return node;
}

void SimplePathing::loadPath(Entity const &from, Entity const &to)
{
	m_path = AStar::singleton().getPath(from, to);
}

std::vector<const DirectX::SimpleMath::Vector3*>& SimplePathing::getPath()
{
	return m_path;
}

int SimplePathing::getCurrentNode() const
{
	return m_currentNode;
}

bool SimplePathing::pathIsEmpty() const
{
	return m_path.empty();
}

bool SimplePathing::pastLastNode() const
{
	return m_path.size() == m_currentNode;
}
