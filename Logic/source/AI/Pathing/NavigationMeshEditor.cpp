#include <AI\Pathing\NavigationMeshEditor.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Structs.h>

#include <d3d11.h>
#include <DirectXMath.h>

using namespace Logic;

NavigationMeshEditor::NavigationMeshEditor()
{
    ray.color = DirectX::SimpleMath::Color ( 0.5f, 0.5f, 0.5f );
    ray.points = newd std::vector<DirectX::SimpleMath::Vector3>();
    ray.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    ray.useDepth = false;
}

NavigationMeshEditor::~NavigationMeshEditor()
{
}

bool NavigationMeshEditor::editNavigationMesh(NavigationMesh &mesh, btVector3 &pos, btVector3 &forward)
{
    using namespace DirectX::SimpleMath;
    auto &mouseState = DirectX::Mouse::Get().GetState();

    // todo delay
    if (mouseState.leftButton) {
        int index = mesh.getIndex(Vector3(pos), Vector3(forward));

        ray.points->clear();
        ray.points->push_back(Vector3(pos));
        ray.points->push_back(Vector3(pos) + Vector3(forward) * 250.f);

        if (index > -1) {
            mesh.removeTriangle(index);
            return true;
        }
    }

    if (!lastState.rightButton && mouseState.rightButton) {
        lastIndex = mesh.getIndex(Vector3(pos), Vector3(forward));
        if (lastIndex > -1) printf("Index targeted: %d\n", lastIndex);
    }
    else if (lastState.rightButton && !mouseState.rightButton) {
        int index = mesh.getIndex(Vector3(pos), Vector3(forward));
        if (index > -1) {
            printf("Index targeted: %d, other index: %d\n", index, lastIndex);
            auto const &nodes = mesh.getNodes();
            mesh.addDoubleEdge(index, lastIndex, nodes[lastIndex] + ((nodes[index] - nodes[lastIndex]) * 0.5f));
        }
    }

    QueueRender(ray);

    lastState = mouseState;
    return false;
}