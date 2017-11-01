#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <Graphics\include\RenderQueue.h>

#include <Entity\PhysicsObject.h>

namespace Logic
{
	class StaticObject : public PhysicsObject
	{
	public:
		StaticObject(Resources::Models::Files modelId, btRigidBody* body, btVector3 halfExtent);

		virtual ~StaticObject();
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void update(float deltaTime);

        void renderD();

        virtual void render() const;

    private:
        StaticRenderInfo staticRenderInfo;
        DebugRenderInfo debugRenderInfo;
    
    };
}

#endif // !STATICOBJECT_H
