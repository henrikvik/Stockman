#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "StatusManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Logic
{
	class Entity : public Object
	{
	public:
		Entity(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

		void destroyBody();
		virtual void clear();
		virtual void update(float deltaTime);
		void updateGraphics();
		virtual void updateSpecific(float deltaTime) { }
		void collision(Entity& other, const btRigidBody* collidedWithYour);
		virtual void affect(int stacks, Effect const &effect, float deltaTime);	
		virtual void upgrade(Upgrade const &upgrade);
		virtual void onCollision(Entity& other, const btRigidBody* collidedWithYour) { }

		void setHalfExtent(btVector3 halfExtent);
		btVector3 getHalfExtent() const;
		DirectX::SimpleMath::Vector3 getPosition() const;
		btVector3 getPositionBT() const;
		DirectX::SimpleMath::Quaternion getRotation() const;
		DirectX::SimpleMath::Vector3 getScale() const;
		DirectX::SimpleMath::Matrix getTransformMatrix() const;

		btRigidBody* getRigidbody();
		StatusManager& getStatusManager();
		void setStatusManager(StatusManager& statusManager);
	private:

		StatusManager m_statusManager;
		btVector3 m_halfextent;
		btRigidBody* m_body;
		btTransform* m_transform;
	};

	//class TestHeadShot : public Entity
	//{
	//public:
	//	TestHeadShot(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE) : Entity(body, halfExtent, modelID) 
	//	{
	//		// Debug draw ray
	//		renderDebug.points = new std::vector<DirectX::SimpleMath::Vector3>;
	//		renderDebug.color = DirectX::SimpleMath::Color(1, 1, 1);
	//		renderDebug.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	//		renderDebug.useDepth = true;
	//	}
	//	
	//	~TestHeadShot() { delete renderDebug.points; }

	//	void updateSpecific(float deltaTime) { }
	//	void onCollision(Entity& other) { }
	//	void render(Graphics::Renderer& renderer)
	//	{
	////		Object::render(renderer);

	//		renderDebug.points->clear();

	//		btRigidBody* rb = this->getRigidbody();
	//		btVector3 dimensions = this->getHalfExtent();
	//		btVector3 position = this->getPositionBT();

	//		btCollisionShape* cs = rb->getCollisionShape();
	//		btBoxShape* bs = dynamic_cast<btBoxShape*>(cs);

	//		btVector3 vp = {0, 0, 0};
	//		
	//		for (int i = 0; i < bs->getNumVertices(); i++)
	//		{
	//			for (int j = 0; j < bs->getNumVertices(); j++)
	//			{
	//				bs->getVertex(i, vp);
	//				renderDebug.points->push_back(DirectX::SimpleMath::Vector3(position + vp));
	//				bs->getVertex(j, vp);
	//				renderDebug.points->push_back(DirectX::SimpleMath::Vector3(position + vp));
	//			}
	//		}
	//			head->setVo
	//		renderer.queueRenderDebug(&renderDebug);
	//	}
	//	

	//private:
	//	btRigidBody* head;
	//	Graphics::RenderDebugInfo renderDebug;
	//};
}

#endif // !ENTITY_H
