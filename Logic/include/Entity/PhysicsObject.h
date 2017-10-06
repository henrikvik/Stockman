#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <Entity/Object.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Logic
{
	class PhysicsObject : public Object
	{
	public:
		PhysicsObject(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE);
		PhysicsObject(const PhysicsObject& other) = delete;
		PhysicsObject* operator=(const PhysicsObject& other) = delete;
		virtual ~PhysicsObject();

		void addExtraBody(btRigidBody* body, float multiplier, btVector3 offset);
		void destroyBody();
		void updatePhysics(float deltaTime);

		void collision(PhysicsObject& other, btVector3 contactPoint, const btRigidBody* collidedWithYour);
		virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier) = 0;

		btVector3 getHalfExtent() const;
		btVector3 getPositionBT() const;

		DirectX::SimpleMath::Vector3 getPosition() const;
		DirectX::SimpleMath::Quaternion getRotation() const;
		DirectX::SimpleMath::Vector3 getScale() const;
		DirectX::SimpleMath::Matrix getTransformMatrix() const;

		btRigidBody* getRigidBody();
		btTransform& getTransform();

		int getNumberOfWeakPoints() const;
		btRigidBody* getRigidBodyWeakPoint(int i);

	private:
		btVector3 m_halfextent;										//< The scaling for the graphical side
		btRigidBody* m_body;										//< The main rigidbody of this physics object
		btTransform* m_transform;									//< Easy acces to the transform
		btVector3 m_weakPointOffset;								//< The constant offset from the body 
		std::vector<std::pair<btRigidBody*, float>> m_weakPoints;	//< The "head" for headshots multiplier change this to (std::pair<btRigidBody*, float multiplier) when you got time
	};
}


#endif // !PHYSICSOBJECT_H
