#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <Entity\PhysicsObject.h>

namespace Graphics
{
    class Renderer;
    struct RenderDebugInfo;
}

namespace Logic
{
	class StaticObject : public PhysicsObject
	{
	public:
        enum NavigationMeshFlags // this is a temporary system for handling different type of obstacles
        {
            NO_CULL = 0x0,
            CULL    = 0x1,
            JUMP_TO = 0x2
        };

		StaticObject(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent, NavigationMeshFlags flags);
		virtual ~StaticObject();
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void update(float deltaTime);

        void renderD(Graphics::Renderer& renderer);
        NavigationMeshFlags getNavFlags() const;
    private:
        Graphics::RenderDebugInfo* renderDebug;
        NavigationMeshFlags m_navFlags;
    };
}

#endif // !STATICOBJECT_H
