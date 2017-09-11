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
		} 

		Shape		shape;			// Type of shape
		btScalar	mass;			// Weight
		btVector3	position;		// Starting position
		btVector3	velocity;		// Starting velocity

		btScalar	yaw;			
		btScalar	pitch;
		btScalar	roll;

		// Must pick one of these
		btVector3	boxDimensions;	// Rectangle
		float		radius;			// Sphere
	};
}