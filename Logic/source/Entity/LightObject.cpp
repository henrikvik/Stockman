#include <Entity\LightObject.h>
#include <Graphics\include\Renderer.h>

using namespace Logic;

LightObject::LightObject(FrameLight& frame)
{
    color       = frame.color;
    intensity   = frame.intensity;
    positionWS  = frame.position;
    range       = frame.range;

    // Render Debug Construction
    renderDebug.points = new std::vector<DirectX::SimpleMath::Vector3>();
    renderDebug.color = DirectX::SimpleMath::Color(color);
    renderDebug.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    renderDebug.useDepth = true;

    // Box
    DirectX::SimpleMath::Vector3 pos = positionWS, size(LIGHT_DEBUG_BOX_SIZE);
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
LightObject::~LightObject() 
{
    renderDebug.points->clear();
    delete renderDebug.points;
}

void LightObject::renderD(Graphics::Renderer& renderer)
{
    renderer.queueRenderDebug(&renderDebug);
}

void LightObject::render(Graphics::Renderer& renderer) 
{ 
    renderer.queueLight(*this); 
}