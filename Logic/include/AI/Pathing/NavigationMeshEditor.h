#include "NavigationMesh.h"
#include <Graphics\include\RenderQueue.h>
#include <Mouse.h>
#include <btBulletCollisionCommon.h>

namespace Logic {
    class NavigationMeshEditor {
        private:
            DebugRenderInfo ray;
            DirectX::Mouse::State lastState;
            int lastIndex;
        public:
            NavigationMeshEditor();
            ~NavigationMeshEditor();

            // returns true if mesh is changed
            bool editNavigationMesh(NavigationMesh &mesh, btVector3 &pos, btVector3 &forward);
    };
}