#include <Entity\LightObject.h>
#include <Graphics\include\Renderer.h>

using namespace Logic;

LightObject::LightObject(FrameLight& frame)
{
    renderInfo.color = DirectX::SimpleMath::Color(frame.color.x, frame.color.y, frame.color.z, 1);
    renderInfo.intensity = frame.intensity;
    renderInfo.position = frame.position;
    renderInfo.range = frame.range;

    // Render Debug Construction
    debugRenderInfo.points = new std::vector<DirectX::SimpleMath::Vector3>();
    debugRenderInfo.color = DirectX::SimpleMath::Color(renderInfo.color);
    debugRenderInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    debugRenderInfo.useDepth = true;

    // Box
    DirectX::SimpleMath::Vector3 pos(renderInfo.position);
    DirectX::SimpleMath::Vector3 size(frame.range);
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
}
LightObject::~LightObject() 
{
    debugRenderInfo.points->clear();
    delete debugRenderInfo.points;
}

void LightObject::renderD()
{
    RenderQueue::get().queue(&debugRenderInfo);
}

void LightObject::render() const
{ 
    RenderQueue::get().queue(&renderInfo);
}