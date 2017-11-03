#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <vector>
#include <Entity/Object.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Logic
{
	class PhysicsObject : public Object
	{
	public:
		struct Weakpoint
		{
			Weakpoint(btRigidBody* inBody, float inMultiplier, btVector3 inOffset)
				: body(inBody), multiplier(inMultiplier), offset(inOffset) { }

			btRigidBody*	body;
			btVector3		offset;
			float			multiplier;
		};


		PhysicsObject(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID);
		PhysicsObject(const PhysicsObject& other) = delete;
		PhysicsObject* operator=(const PhysicsObject& other) = delete;
		virtual ~PhysicsObject();

		void addExtraBody(btRigidBody* body, float multiplier, btVector3 offset);
		void destroyBody();
		void updatePhysics(float deltaTime);

		void collision(PhysicsObject& other, btVector3 contactPoint);
		virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier) = 0;

        virtual void setHalfExtent(btVector3 halfExtent);
		virtual btVector3 getHalfExtent() const;
		virtual btVector3 getPositionBT() const;

		virtual DirectX::SimpleMath::Vector3 getPosition() const;
		DirectX::SimpleMath::Quaternion getRotation() const;
		DirectX::SimpleMath::Vector3 getScale() const;
		virtual DirectX::SimpleMath::Matrix getTransformMatrix() const;

		btRigidBody* getRigidBody();
		virtual btTransform& getTransform();

		int getNumberOfWeakPoints() const;
		btRigidBody* getRigidBodyWeakPoint(int i);

	private:
		btVector3 m_halfextent;										//< The scaling for the graphical side
		btRigidBody* m_body;										//< The main rigidbody of this physics object
		btTransform* m_transform;									//< Easy acces to the transform
		std::vector<Weakpoint> m_weakPoints;						//< The "head" for headshots multiplier change this to (std::pair<btRigidBody*, float multiplier) when you got time
	};
}


#endif // !PHYSICSOBJECT_H
