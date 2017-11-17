#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <d3d11.h>
#include <SimpleMath.h>

#include <vector>
#include <Entity/Object.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Physics\FuncContactResult.h>

namespace Logic
{
    class Physics;
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


        PhysicsObject(btRigidBody* body, btVector3 halfExtent, btVector3 modelOffset = { 0.f, 0.f, 0.f });
		PhysicsObject(const PhysicsObject& other) = delete;
		PhysicsObject* operator=(const PhysicsObject& other) = delete;
		virtual ~PhysicsObject();

		void addExtraBody(btRigidBody* body, float multiplier, btVector3 offset);
		void destroyBody();
		void updatePhysics(float deltaTime);

		void collision(PhysicsObject& other, btVector3 contactPoint, Physics &physics);
		virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier) = 0;

        virtual void setModelOffset(btVector3 modelOffset);
        virtual void setHalfExtent(btVector3 halfExtent);
		virtual btVector3 getHalfExtent() const;
		virtual btVector3 getPositionBT() const;

		virtual DirectX::SimpleMath::Vector3 getPosition() const;
		DirectX::SimpleMath::Quaternion getRotation() const;
		DirectX::SimpleMath::Vector3 getScale() const;
		virtual DirectX::SimpleMath::Matrix getTransformMatrix() const;
        virtual DirectX::SimpleMath::Matrix getModelTransformMatrix() const;

		btRigidBody* getRigidBody();
		virtual btTransform& getTransform();

		int getNumberOfWeakPoints() const;
		btRigidBody* getRigidBodyWeakPoint(int i);

    protected:
        btRigidBody* m_body;										//< The main rigidbody of this physics object
        btTransform* m_transform;									//< Easy acces to the transform
        btVector3 m_modelOffset;                                    //< Model offset
	private:
		btVector3 m_halfextent;										//< The scaling for the graphical side
		std::vector<Weakpoint> m_weakPoints;						//< The "head" for headshots multiplier change this to (std::pair<btRigidBody*, float multiplier) when you got time
	};
}


#endif // !PHYSICSOBJECT_H
