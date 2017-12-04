#include "NavigationMesh.h"
#include <Graphics\include\RenderQueue.h>
#include <Mouse.h>

namespace Logic {
    class NavigationMeshEditor {
        private:
            DebugRenderInfo ray;
            DirectX::Mouse::State lastState;
        public:
            NavigationMeshEditor();
            ~NavigationMeshEditor();

            // returns true if mesh is changed
            bool editNavigationMesh(NavigationMesh &mesh);
    };
}