#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <Graphics\include\RenderQueue.h>
#include <Entity\PhysicsObject.h>

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

		StaticObject(Resources::Models::Files modelId, btRigidBody* body, btVector3 halfExtent, NavigationMeshFlags flags);

		virtual ~StaticObject();
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void update(float deltaTime);


        NavigationMeshFlags getNavFlags() const;
        void renderD();
        void render() const;
    private:
        NavigationMeshFlags m_navFlags;
        StaticRenderInfo staticRenderInfo;
        DebugRenderInfo debugRenderInfo;
    };
}

#endif // !STATICOBJECT_H
