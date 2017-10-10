#include <AI\Behavior\TestBehavior.h> 
#include <AI\Enemy.h>

using namespace Logic;

TestBehavior::TestBehavior()
{
	debugInfo.color = DirectX::SimpleMath::Color(1, 0, 0);
	debugInfo.useDepth = true;
	debugInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	debugInfo.points = new std::vector<DirectX::SimpleMath::Vector3>();
}

TestBehavior::~TestBehavior()
{
	if (debugInfo.points)
		delete debugInfo.points;
}

void TestBehavior::update(Enemy &enemy, std::vector<Enemy*> const &close,
	Player const & player, float deltaTime)
{
	btVector3 node = m_path.updateAndReturnCurrentNode(enemy, player);
	btVector3 dir = node - enemy.getPositionBT();

	dir = dir.normalize();
	dir *= deltaTime / 1000.f;
	dir *= enemy.getMoveSpeed();

	if (enemy.getHealth() < 5)
	{
		enemy.getRigidBody()->applyCentralForce(dir * -20000);
	}
		else 
	{
		enemy.getRigidBody()->translate(dir);
	}

	if ((node - enemy.getPositionBT()).length() < 0.8f)
		m_path.setCurrentNode(m_path.getCurrentNode() + 1);
}

void TestBehavior::updatePath(Entity const &from, Entity const &to)
{
	m_path.loadPath(from, to);
	m_path.setCurrentNode(0);

	debugInfo.points->clear();
	debugInfo.points->push_back(from.getPosition());
	for (auto *vec : m_path.getPath())
	{
		debugInfo.points->push_back(*vec);
	}
}

void TestBehavior::debugRendering(Graphics::Renderer &renderer)
{
	if (debugInfo.points)
		renderer.queueRenderDebug(&debugInfo);
}
