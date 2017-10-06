#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <Entity\Entity.h>

namespace Logic
{
	class StaticObject : public PhysicsObject
	{
	public:
		StaticObject(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent);
		virtual ~StaticObject();
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void update(float deltaTime);
	};
}

#endif // !STATICOBJECT_H
