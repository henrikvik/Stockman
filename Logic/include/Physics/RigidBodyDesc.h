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
		BodyDesc(btScalar mass, btVector3 position, btVector3 boxDimensions)
		{
			this->shape			= ShapeBox;
			this->mass			= mass;
			this->position		= position;
			this->rotation		= { 0, 0, 0 };
			this->restitution	= 0.5f;
			this->friction		= 0.5f;
			this->velocity		= { 0, 0, 0 };

			// Box Specifics
			this->boxDimensions = boxDimensions;
		}

		// Static Plane Description
		BodyDesc(btScalar mass, btVector3 position, btScalar scalar, btVector3 normal)
		{
			this->shape			= ShapeStaticPlane;
			this->mass			= mass;
			this->position		= position;
			this->rotation		= { 0, 0, 0 };
			this->restitution	= 0.5f;
			this->friction		= 0.5f;
			this->velocity		= { 0, 0, 0 };

			// Plane Specifics
			this->scalar		= scalar;
			this->normal		= normal;
		}

		// Sphere Description
		BodyDesc(btScalar mass, btVector3 position, btScalar radius)
		{
			this->shape			= ShapeSphere;
			this->mass			= mass;
			this->position		= position;
			this->rotation		= { 0, 0, 0 };
			this->restitution	= 0.5f;
			this->friction		= 0.5f;
			this->velocity		= { 0, 0, 0 };

			// Sphere Specifics
			this->radius		= radius;
		}

		Shape				shape;
		btScalar			mass;
		btVector3			position;
		btVector3			rotation;
		btScalar			restitution;	// Bounciness
		btScalar			friction;
		btVector3			velocity;
		
		btVector3			normal;
		btScalar			scalar;
		float				radius;
		btVector3			boxDimensions;	// Rectangle
	};
}