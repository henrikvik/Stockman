#include <btBulletCollisionCommon.h>

namespace Logic
{
	enum Shape 
	{
		ShapeBox,			
		ShapeSphere,
		ShapeStaticPlane
	};
	
	struct BodyDesc
	{
		// Simple Box Description
		// - Dynamic body, intractable
		BodyDesc(btScalar mass, btVector3 position, btVector3 boxDimensions)
		{
			this->shape			= ShapeBox;
			this->mass			= mass;
			this->position		= position;
			this->rotation		= { 0, 0, 0 };
			this->restitution	= 0.0f;
			this->friction		= 1.0f;
			this->velocity		= { 0, 0, 0 };

			// Box Specifics
			this->boxDimensions = boxDimensions;
		}

		// Static Plane Description
		// - Static body, non-intractable
		BodyDesc(btVector3 position, btScalar scalar, btVector3 normal)
		{
			this->shape			= ShapeStaticPlane;
			this->mass			= 0.f;				// static objects needs to be mass = 0.f
			this->position		= position;
			this->rotation		= { 0, 0, 0 };
			this->restitution	= 0.0f;
			this->friction		= 1.0f;
			this->velocity		= { 0, 0, 0 };

			// Plane Specifics
			this->scalar		= scalar;
			this->normal		= normal;
		}

		// Sphere Description
		// - Dynamic body, intractable
		BodyDesc(btScalar mass, btVector3 position, btScalar radius)
		{
			this->shape			= ShapeSphere;
			this->mass			= mass;
			this->position		= position;
			this->rotation		= { 0, 0, 0 };
			this->restitution	= 0.0f;
			this->friction		= 1.0f;
			this->velocity		= { 0, 0, 0 };

			// Sphere Specifics
			this->radius		= radius;
		}

		Shape				shape;
		btScalar			mass;
		btVector3			position;
		btVector3			rotation;
		btScalar			restitution;	// 0 - brick, 1 - rubberball
		btScalar			friction;		// 0 - ice, 1 - rock
		btVector3			velocity;		
		
		btVector3			normal;
		btScalar			scalar;
		float				radius;
		btVector3			boxDimensions;	// Rectangle
	};
}