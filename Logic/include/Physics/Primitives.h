#include <btBulletCollisionCommon.h>

namespace Logic
{
	enum ShapeType
	{
		ShapeTypePoint,
		ShapeTypeCube,			
		ShapeTypeSphere,
		ShapeTypePlane,
		ShapeTypeCylinder, 
		ShapeTypeCapsule
	};


	////////////////////////////////////////////////////////////
	// A simple ray
	//	Needs to have both a start and end
	////////////////////////////////////////////////////////////
	class Ray
	{
	public:
		Ray(btVector3 start, btVector3 end)
		{
			m_start = start;
			m_end = end;
		}

		Ray(btVector3 start, btVector3 forward, btScalar length)
		{
			m_start = start;
			m_end = start + (forward * length);
		}

		const	btVector3& getStart() 			{ return m_start;	}
		const	btVector3& getEnd()				{ return m_end;		}
		void	setStart(btVector3 start)		{ m_start = start;	}
		void	setEnd(btVector3 end)			{ m_end = end;		}

	private:
		btVector3	m_start;
		btVector3	m_end;
	};

	class Triangle
	{
	public:
		Triangle(btVector3 p1, btVector3 p2, btVector3 p3)
		{
		//	memcpy(point, (btVector3[3]) { p1, p2, p3 }, 3 * sizeof(btVector3));
		}

		btVector3 operator[] (int i)
		{
			return point[i];
		}

	private:
		btVector3 point[3];
	};

	////////////////////////////////////////////////////////////
	// Base shape primitive
	//  No use on it's own
	////////////////////////////////////////////////////////////
	class Shape
	{
	public:
		Shape(btVector3 position, btVector3 rotation)
		{
			m_shape = ShapeTypePoint;
			m_position = position;
			m_rotation = rotation;
		}

		void		setPos(btVector3 position)	{ m_position = position;	}
		void		setRot(btVector3 rotation)	{ m_rotation = rotation;	}
		btVector3	getPos() const				{ return m_position;		}
		btVector3	getRot() const				{ return m_rotation;		}

	protected:
		ShapeType	m_shape;
	private:
		btVector3	m_position;
		btVector3	m_rotation;
	};


	////////////////////////////////////////////////////////////
	//	Creates a simple 3D cube 
	////////////////////////////////////////////////////////////
	class Cube : public Shape
	{
	public:
		Cube(btVector3 position, btVector3 rotation, btVector3 dimensions)
		: Shape(position, rotation) {
			m_dimensions = dimensions;
			m_shape = ShapeTypeCube;
		}

		void		setDimensions(btVector3 dimensions) { m_dimensions = dimensions;	}
		btVector3	getDimensions() const				{ return m_dimensions;			}

	private:
		btVector3	m_dimensions;
	};

	////////////////////////////////////////////////////////////
	// Creates a 2D plane which is always infinite in every situation
	//	Mostly used for testing 
	////////////////////////////////////////////////////////////
	class Plane : public Shape
	{
	public:
		Plane(btVector3 normal)
			: Shape({ 0, 0, 0 }, { 0, 0, 0 }) {
			m_shape = ShapeTypePlane;
			m_normal = normal;
		}

		void		setNormal(btVector3 normal)	{ m_normal = normal;	}
		btVector3	getNormal() const			{ return m_normal;		}

	private:
		btVector3	m_normal;	//< Direction of the plane
	};

	////////////////////////////////////////////////////////////
	// Creates a 3D sphere 
	////////////////////////////////////////////////////////////
	class Sphere : public Shape
	{
	public:
		Sphere(btVector3 position, btVector3 rotation, float radius)
			: Shape(position, rotation) {
			m_radius = radius;
			m_shape = ShapeTypeSphere;
		}

		void	setRadius(float radius)		{ m_radius = radius;	}
		float	getRadius() const			{ return m_radius;		}

	private:
		float		m_radius;
	};

	////////////////////////////////////////////////////////////
	// Creates a 3D Cylinder 
	//  Centered around the origin, central axis around the y-axis
	////////////////////////////////////////////////////////////
	class Cylinder : public Shape
	{
	public:
		Cylinder(btVector3 position, btVector3 rotation, btVector3 halfExtends)
			: Shape(position, rotation) {
			m_halfExtends = halfExtends;
			m_shape = ShapeTypeCylinder;
		}

		void setHeight(btVector3 halfExtends)	{ m_halfExtends = halfExtends;	}
		btVector3 getHalfExtends() const		{ return m_halfExtends;			}

	private:
		btVector3 m_halfExtends;
	};

	////////////////////////////////////////////////////////////
	// Creates a 3D Capsule
	//  Capsule around the Y-Axis
	//  Used for characters, it's hard to make a capsule to get stuck in rough areas
	////////////////////////////////////////////////////////////
	class Capsule : public Shape
	{
	public:
		Capsule(btVector3 position, btVector3 rotation, float radius, float height)
		: Shape(position, rotation) {
			m_radius = radius;
			m_height = height;
			m_shape = ShapeTypeCapsule;
		}

		void	setRadius(float radius)		{ m_radius = radius;	}
		void	setHeight(float height)		{ m_height = height;	}
		float	getRadius() const			{ return m_radius;		}
		float	getHeight() const			{ return m_height;		}

	private:
		float m_radius;
		float m_height;
	};
}