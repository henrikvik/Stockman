#include <Entity\StaticObject.h>
#include <Graphics\include\Renderer.h>

using namespace Logic;

StaticObject::StaticObject(Graphics::ModelID modelID, btRigidBody * body, btVector3 halfExtent)
	: PhysicsObject(body, halfExtent, modelID)
{
    // Render Debug Construction
    renderDebug.points = new std::vector<DirectX::SimpleMath::Vector3>();
    renderDebug.color = DirectX::SimpleMath::Color(1, 1, 1);
    renderDebug.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    renderDebug.useDepth = true;

    // Box
    DirectX::SimpleMath::Vector3 pos = getPosition(), size(getHalfExtent());
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z + size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z - size.z));
    renderDebug.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z - size.z));
}

StaticObject::~StaticObject() 
{
    renderDebug.points->clear();
    delete renderDebug.points;
}

void StaticObject::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier) 
{

}

void StaticObject::update(float deltaTime) 
{
	PhysicsObject::updatePhysics(deltaTime);
}

void StaticObject::renderD(Graphics::Renderer& renderer)
{
    renderer.queueRenderDebug(&renderDebug);
}
