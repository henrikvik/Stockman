#include <Graphics\include\RenderQueue.h>
#include <Entity\StaticObject.h>

using namespace Logic;

StaticObject::StaticObject(Resources::Models::Files modelID, btRigidBody * body, btVector3 halfExtent, NavigationMeshFlags flags)
	: PhysicsObject(body, halfExtent, modelID), m_navFlags(flags)
{
    // Render Debug Construction
    debugRenderInfo.points = newd std::vector<DirectX::SimpleMath::Vector3>();
    debugRenderInfo.color = DirectX::SimpleMath::Color(1, 1, 1);
    debugRenderInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    debugRenderInfo.useDepth = true;

    // Box
    DirectX::SimpleMath::Vector3 pos = getPosition(), size(getHalfExtent());
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z + size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y + size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x + size.x, pos.y - size.y, pos.z - size.z));
    debugRenderInfo.points->push_back(DirectX::SimpleMath::Vector3(pos.x - size.x, pos.y - size.y, pos.z - size.z));

    staticRenderInfo.transform = getTransformMatrix();
    staticRenderInfo.model = modelId;
}

StaticObject::~StaticObject()
{
    debugRenderInfo.points->clear();
    delete debugRenderInfo.points;
}

void StaticObject::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier) 
{

}

void StaticObject::update(float deltaTime) 
{
	PhysicsObject::updatePhysics(deltaTime);
}

void StaticObject::renderD()
{
    RenderQueue::get().queue(&debugRenderInfo);
}

void Logic::StaticObject::render() const
{
    RenderQueue::get().queue(&staticRenderInfo);
}

StaticObject::NavigationMeshFlags StaticObject::getNavFlags() const
{
    return m_navFlags;
}
