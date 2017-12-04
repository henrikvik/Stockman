#include <AI\Pathing\NavigationMeshEditor.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Structs.h>

#include <d3d11.h>
#include <DirectXMath.h>

using namespace Logic;

NavigationMeshEditor::NavigationMeshEditor()
{
    ray.color = DirectX::SimpleMath::Color ( 0, 0, 0 );
    ray.points = new std::vector<DirectX::SimpleMath::Vector3>();
    ray.points->resize(2);
    ray.topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
    ray.useDepth = false;
}

NavigationMeshEditor::~NavigationMeshEditor()
{
}

bool NavigationMeshEditor::editNavigationMesh(NavigationMesh &mesh)
{
    using namespace DirectX::SimpleMath;
    // dont look this is awful
    // globals...
    Graphics::Camera *camera = Global::mainCamera;
    auto &mouseState = DirectX::Mouse::Get().GetState();
    auto &pos = camera->getPos();
    auto &forward = camera->getForward();

    // Mouse Picking copied from my proj
    Vector4 mouseRay(mouseState.x * 2 / static_cast<float> (WIN_WIDTH) - 1,
        1 - (mouseState.y * 2 / static_cast<float> (WIN_HEIGHT)),
        1.f, 0.f); //w = 0, is ray, z = 1 because pointing away from screen
    Vector4 mouseOrigin(0, 0, 0, 1); // w = 1 because is POINT

    mouseRay.x /= camera->getProj()._11;
    mouseRay.y /= camera->getProj()._22; //Inverse perspective
    mouseRay = Vector4::Transform(mouseRay, camera->getView().Invert());
    mouseOrigin = Vector4::Transform(mouseOrigin, camera->getView().Invert());

    // todo delay
    if (!mouseState.leftButton && lastState.leftButton) {
        int index = mesh.getIndex({ mouseOrigin.x, mouseOrigin.y, mouseOrigin.z }, { mouseRay.x, mouseRay.y, mouseRay.z });
        ray.points[0] = { mouseOrigin.x, mouseOrigin.y, mouseOrigin.z };
        ray.points[1] = Vector3(mouseOrigin.x, mouseOrigin.y, mouseOrigin.z) + Vector3(mouseRay.x, mouseRay.y, mouseRay.z) * 150.f;

        if (index > -1) {
            printf("Remove: %d\n", index);
            mesh.removeTriangle(index);
            return true;
        }
    }
    QueueRender(ray);

    lastState = mouseState;
    return false;
}