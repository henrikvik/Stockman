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
		StaticObject(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent);
		virtual ~StaticObject();
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void update(float deltaTime);

        void renderD(Graphics::Renderer& renderer);

    private:
        Graphics::RenderDebugInfo* renderDebug;
    
    };
}

#endif // !STATICOBJECT_H
