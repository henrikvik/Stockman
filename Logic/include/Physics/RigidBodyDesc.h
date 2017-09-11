#include <btBulletCollisionCommon.h>

namespace Logic
{
	enum Shape { ShapeSphere, ShapeRectangle, /*ShapeTriangle*/ };

	struct RigidBodyDesc
	{
		// Simple Box Description
		RigidBodyDesc(Shape shape, btScalar mass, btVector3 position, btVector3 boxDimensions) 
		{
			this->shape				= shape;
			this->mass				= mass;
			this->position			= position;
			this->boxDimensions		= boxDimensions;

			this->velocity			= { 0, 0, 0 };
			this->rotation			= { 0, 0, 0 };
			this->restitution		= 0.5f;
			this->friction			= 0.5f;
			this->radius			= 0.f;
		} 

		Shape		shape;			// Type of shape
		btScalar	mass;			
		btVector3	position;		
		btVector3	velocity;		
		btVector3	rotation;		
		btScalar	restitution;	// Bounciness
		btScalar	friction;		

		// Must pick one of these if specific shape
		btVector3	boxDimensions;	// Rectangle
		float		radius;			// Sphere
	};
}