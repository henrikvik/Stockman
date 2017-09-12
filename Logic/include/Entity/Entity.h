#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "StatusManager.h"

#include "../Physics/Physics.h"
#include "../Physics/RigidBodyDesc.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Logic
{
	class Entity : public Object
	{
	public:
		Entity();
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

		bool init(Physics* physics, BodyDesc bodyDesc);
		virtual void createBody(Physics* physics, BodyDesc bodyDesc);

		virtual void clear();
		void update(float deltaTime);
		virtual void updateSpecific(float deltaTime) = 0;
		void collision(Entity& other);
		virtual void onCollision(Entity& other) = 0;

		// JUST FOR TESTING, REMOVE
		void consoleWritePosition();

		DirectX::SimpleMath::Vector3 getPosition();

	private:
		StatusManager m_statusManager;
		btRigidBody* m_body;
	//	Hitbox* m_head; for headshot, put it here to remember

	protected:
		btRigidBody* getRigidbody();
	};
}

#endif // !ENTITY_H
